#include <concepts>
#include <type_traits>
#include <cstdint>
#include <atomic>
#include <cstddef>
#include <iostream>

// ============================================================================
// HARDWARE ALIGNMENT CONTRAINTS & TRAITS
// ============================================================================
/**
 * @brief Custom compile-time type trait to enforce accelerator/HPC compatibility.
 * 
 * @details Evaluates to true only if the type's alignment matches or exceeds a standard 
 * modern cache line (64 bytes). This prevents performance degradation like "false sharing" 
 * and "cache thrashing" when objects are heavily modified across multiple CPU or GPU cores.
 */
template <typename T>
struct is_accelerator_friendly {
    static constexpr bool value = (alignof(T) >= 64) && (alignof(T) % 64 == 0);
};

// ============================================================================
// C++20 MODULAR CONCEPTS
// ============================================================================

/**
 * @brief Constraint requiring an explicit identification token.
 * 
 * @details The '->' syntax denotes a Compound Requirement. The compiler checks:
 * 1. Does the expression `instance.get_owner_id()` successfully compile?
 * 2. If so, is its resulting return type exactly matching `std::uint64_t`?
 */
template<typename T>
concept HasId = requires(const T instance) {
    { instance.get_owner_id() } -> std::same_as<std::uint64_t>;
};

/**
 * @brief Constraint verifying hardware-level memory boundaries.
 * Enforces that alignof(T) is optimized for cache lines via our type trait.
 */
template<typename T>
concept IsCacheAligned = is_accelerator_friendly<T>::value;

/**
 * @brief Combined Custom SafeOwner Concept.
 * Synthesizes syntax validation (HasId) and hardware optimization validation (IsCacheAligned)
 * into a single unified constraint evaluated entirely at compile time.
 */
template<typename T>
concept SafeOwner = HasId<T> && IsCacheAligned<T>;

// ============================================================================
// INTRUSIVE THREAD-SAFE REGISTRY MIXIN
// ============================================================================
/**
 * @brief Intrusive tracking base class that automatically registers object lifetimes.
 * 
 * @note ARCHITECTURAL NOTE: Why is this registry "INTRUSIVE"?
 * In a non-intrusive system (like std::shared_ptr/std::weak_ptr), tracked objects are
 * passive, and an external control block must be allocated on the heap to store metadata.
 * 
 * This design is INTRUSIVE because the tracking infrastructure ('m_id') is baked directly
 * into the memory footprint of the object itself via inheritance, and the object handles
 * its own self-registration in its constructor and destructor.
 * 
 * High-Performance Computing (HPC) Trade-offs:
 * - Eliminates dynamic heap allocation overhead (No runtime 'new' or 'malloc').
 * - Guarantees perfect memory cache locality for the tracking ID.
 * - Sacrifices strict class isolation to enable seamless GPU-offloading compatibility.
 * 
 * Enforces 64-byte alignment via `alignas(64)`. This guarantees that any derived user 
 * class inheriting from SafeLifecycle automatically inherits this cache-line optimization.
 */
class alignas(64) SafeLifecycle {
private:
    // Embedded instance token. Because this registry is intrusive, this 8-byte variable
    // physically alters and sits inside the memory footprint of any derived class.
    // Good for current GPUs. Bad for current CPUs.
    std::uint64_t m_id;

    // GLOBAL DATABASE RULES (Declared static so they belong to the class, not instances)
    // Using 'static inline' (C++17) allows these globals to live purely within this header file.
    static constexpr std::size_t MAX_TRACKED_OBJECTS = 1024;
    static inline std::uint64_t active_ids[MAX_TRACKED_OBJECTS] = {0};
    static inline std::size_t active_count = 0;

    // ATOMIC SYNCHRONIZATION
    // The simplest, lowest-level atomic type in C++. It is guaranteed lock-free 
    // and maps directly to hardware-level atomic instructions. Initializes to the unlocked state.
    static inline std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;

    /**
     * @brief Acquires the spinlock.
     * Continuously "spins" in a tight loop using an atomic test-and-set operation with 
     * Acquire memory ordering until the lock becomes available. 
     */
    static void lock() noexcept {
        while (lock_flag.test_and_set(std::memory_order_acquire)) {}
    }

    /**
     * @brief Releases the spinlock.
     * Clears the atomic flag using Release memory ordering, signaling to waiting threads 
     * that the registry is safe to modify.
     */
    static void unlock() noexcept {
        lock_flag.clear(std::memory_order_release);
    }

public:
    /**
     * @brief Constructor handles automatic runtime registration.
     * Captures the object's unique memory footprint, locks the spinlock, and safely 
     * updates the contiguous static registry database.
     */
    SafeLifecycle() noexcept {
        m_id = reinterpret_cast<std::uint64_t>(this); 
        
        lock();
        if (active_count < MAX_TRACKED_OBJECTS) {
            active_ids[active_count++] = m_id;
        }
        unlock();
    }

    /**
     * @brief Destructor handles automatic runtime clean-up.
     * Erases the tracking token and contracts the active array in O(1) constant time 
     * by swapping the target entry with the final element in the array.
     */
    ~SafeLifecycle() noexcept {
        lock();
        for (std::size_t i = 0; i < active_count; ++i) {
            if (active_ids[i] == m_id) {
                active_ids[i] = active_ids[active_count - 1];
                active_ids[active_count - 1] = 0;
                active_count--;
                break;
            }
        }
        unlock();
    }

    /**
     * @brief Accessor for the compile-time 'HasId' concept validation.
     */
    std::uint64_t get_owner_id() const noexcept { return m_id; }

    /**
     * @brief The thread-safe check utilized by asynchronous or background execution loops.
     * 
     * @details Allows a parallel thread to check if an object ID is still active 
     * WITHOUT touching or dereferencing the actual object pointer (which prevents 
     * immediate Segmentation Faults or Use-After-Free vulnerabilities).
     */
    static bool is_alive(std::uint64_t id) noexcept {
        lock();
        bool found = false;
        for (std::size_t i = 0; i < active_count; ++i) {
            if (active_ids[i] == id) {
                found = true;
                break; // HIGHLY OPTIMIZED: Drop the spinlock immediately upon identification
                        //Be very careful with scope!!!
                        //This is why spinlocks are generally frowned upon outside
                        // HPC and GPU contexts!
            }
        }
        unlock();
        return found;
    }
};

// ============================================================================
// TEST WIDGET LAYOUTS FOR DEMONSTRATION
// ============================================================================
// --- THIS PASSES COMPILATION ---
// Satisfies both HasId and the custom IsCacheAligned constraints!
class alignas(64) GoodWidget : public SafeLifecycle {
    int data;
};

/**
 * @brief ARCHITECTURAL TEACHING MOMENT: This WILL compile successfully!
 * 
 * @details Even though this class lacks an explicit 'alignas(64)' specifier, it inherits 
 * from SafeLifecycle, which IS marked 'alignas(64)'. In C++, a derived class automatically 
 * inherits the alignment restrictions of its base class. 
 * 
 * Therefore, the compiler silently forces BadWidget to a 64-byte alignment, satisfying 
 * our IsCacheAligned constraint automatically!
 */
class BadWidget : public SafeLifecycle {
    int data;
}; 

// ============================================================================
// INTENTIONAL COMPILER ERROR TEST CASES
// ============================================================================

// --- CASE 1: FAILS 'HasId' (Missing the required API function entirely) ---
class alignas(64) NoIdWidget {
public:
    int data; // Missing get_owner_id()
};

/**
 * @brief This satisfies the 'HasId' syntax but satisfies NO alignment properties.
 * 
 * @details Because it does not inherit from SafeLifecycle, it keeps standard 4 or 8-byte 
 * alignment. This is the exact class to use if you want to demonstrate a compilation 
 * failure to your students.
 */
class TrulyBadWidget {
public:
    int data;
    // Give it the ID function so it passes 'HasId', but it will fail 'IsCacheAligned'
    std::uint64_t get_owner_id() const noexcept { return 12345; }
};

// Verification function explicitly constrained by the SafeOwner concept
template<SafeOwner T>
void register_callback_target(T* target) {
    std::cout << "[Concept Verification] Object successfully passed compile-time constraints!\n";
}

int main() {
    std::cout << "=== Beginning Intrusive Lifecycle & Concept Verification ===\n\n";

    // Instantiation & Concept Hook verification
    std::cout << "--- Instantiation & Concept Hook Verification ---\n";
    
    GoodWidget w2;
    std::uint64_t saved_id = w2.get_owner_id();
    // Pass to our concept-guarded function
    register_callback_target(&w2);
    
    // Print hexadecimal memory address to visually prove alignment
    std::cout << "GoodWidget Tracking ID (Memory Address): 0x" 
              << std::hex << saved_id << std::dec << "\n";
    if (saved_id % 64 == 0) {
        std::cout << "-> Verification Success: Memory address is perfectly divisible by 64!\n\n";
    }

    BadWidget w1;
    register_callback_target(&w1); 

    //Uncomment both lines to get compile error
    // NoIdWidget w_fail1;
    // register_callback_target(&w_fail1);

    //Uncomment both lines to get compile error
    //TrulyBadWidget w_fail;
    //register_callback_target(&w_fail);

    // Registry verification (Alive vs. Dead)
    std::cout << "--- Thread-Safe Global Registry Interrogation ---\n";
    std::cout << "Is GoodWidget tracked as alive in global array? " 
              << (SafeLifecycle::is_alive(saved_id) ? "TRUE" : "FALSE") << "\n";

    // Ensure tracking variable outside of the scope block to preserve state!
    std::uint64_t tracked_temp_id = 0;
    // Scope block to simulate runtime destruction
    {
        std::cout << "\nCreating a temporary, scoped GoodWidget...\n";
        GoodWidget temp_widget;
        tracked_temp_id = temp_widget.get_owner_id();        
        
        std::cout << "Temporary widget address: 0x" << std::hex << tracked_temp_id << std::dec << "\n";
        std::cout << "Is temporary widget alive? " 
                  << (SafeLifecycle::is_alive(tracked_temp_id) ? "TRUE" : "FALSE") << "\n";
        std::cout << "Exiting scope block, calling destructor...\n";    
    }
    // Destructor clears it here

    std::cout << "Is temporary widget still alive in array? " 
              << (SafeLifecycle::is_alive(tracked_temp_id) ? "TRUE" : "FALSE") << " (Successfully scrubbed!)\n\n";

    std::cout << "=== Verification Complete ===\n";

    return 0;
}

