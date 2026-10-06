#include <iostream>
#include <string>
#include <concepts>
#include <memory>
#include <vector>

// The concept we want to erase
template <class T>
concept ILabel = requires(T v) {
    {v.buildHtml()} -> std::convertible_to<std::string>;
};

// the Type Erasure Wrapper
// This class acts as a single concrete 'Value Type' that hides (erases) the 
// specific type of any object passed into it, providing value semantics.
class AnyLabel {
private:
    // the hidden blueprint (abstract base class interface)
    struct LabelInterface {
        // Virtual destructor guarantees that when unique_ptr deletes this 
        // interface pointer, the actual concrete derived class's destructor runs.
        virtual ~LabelInterface() = default;
        //target hook for type execution
        virtual std::string callBuildHtml() const = 0;
    };
    
    // Generic contaner that implements the blueprint for a specific type T
     // The constraint 'template <ILabel T>' catches non-conforming classes at compile-time.
    template <ILabel T>
    struct LabelImplementation : LabelInterface {
        T concreteObject; // Stores the eraseable object by value on the heap.

        // Constructor moves or copies the external object into our internal storage.
        LabelImplementation(T value) : concreteObject(std::move(value)) {}

        // Overrides the virtual hook to bridge run-time dynamic dispatch 
        // with compile-time template duck typing.
        std::string callBuildHtml() const override {
            return concreteObject.buildHtml();
        }
    };

    // Pointer that manages the hidden object
    std::unique_ptr<LabelInterface> hiddenObject;

public:
    // Constructor pulls any valid ILabel into our implementation container
    template <ILabel T>
    AnyLabel(T value)
        : hiddenObject(std::make_unique<LabelImplementation<T>>(std::move(value))) {}
    
    // Expose clean interface to outside world
    std::string buildHtml() const {
        return hiddenObject->callBuildHtml();
    }
};

// PASSES - perfect match, returns a std::string
class TextLabel {
public: 
    std::string buildHtml() const { return "<p>Hello from the wide world of sports.</p>";}
};

struct CustomLabel { std::string buildHtml() const { return "<h1>Custom</h1>";} };
struct AnontherLabel { std::string buildHtml() const { return "<p>Another</P>";} };
class QuickLabel {
public:
    const char* buildHtml() const { return "<div>Quick</div>";}
};

// For Failure Object Slicing
class SlicedBaseLabel {
public:
    std::string buildHtml() const { return "<p>Base Label</p>"; }
};
class SlicedDerivedLabel : public SlicedBaseLabel {
public:
    std::string buildHtml() const { return "<div>Derived Label (Rich Data)</div>"; }
};

// Helper function demonstrating the slicing trap
void registerSlicedLabel(const SlicedBaseLabel& baseRef, std::vector<AnyLabel>& vec) {
    // TRAP: baseRef has a static type of SlicedBaseLabel.
    // This creates an AnyLabel holding a SlicedBaseLabel, discarding the Derived class data!
    vec.push_back(baseRef); 
}

// For Failure Non-Movable / Non-Copyable
class NonMovableLabel {
public:
    NonMovableLabel() = default;
    NonMovableLabel(const NonMovableLabel&) = delete; // Disables copies
    NonMovableLabel(NonMovableLabel&&) = delete;      // Disables moves
    std::string buildHtml() const { return "<span>Locked Label</span>"; }
};

int main() {
    std::vector<AnyLabel> layout;

    layout.push_back(TextLabel{});
    layout.push_back(CustomLabel{});
    layout.push_back(AnontherLabel{});
    layout.push_back(TextLabel{});

    // FAILURE TYPE: RUN-TIME TRAP (Object Slicing)
    SlicedDerivedLabel derivedLabel;
    registerSlicedLabel(derivedLabel, layout); // Pass derived class into base reference

    // FAILURE TYPE: COMPILE-TIME ERROR (Non-Movable Types)
    NonMovableLabel secretLabel;
    // UNCOMMENT TO FAIL: AnyLabel requires its payload to be movable into the heap container.
    //layout.push_back(std::move(secretLabel)); 

    // FAILURE TYPE: COMPILE-TIME ERROR (Vector Copying)
    // UNCOMMENT TO FAIL: AnyLabel contains a std::unique_ptr (move-only).
    // Because the elements can't be copied, you cannot copy the vector itself.
    //std::vector<AnyLabel> layoutCopy = layout;

    for (const auto& label : layout) {
        std::cout << label.buildHtml() << "\n";
    }
    
    return 0;
}
   