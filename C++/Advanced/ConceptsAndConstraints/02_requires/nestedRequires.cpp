#include <iostream>
#include <concepts>
#include <type_traits>

// Concept Definition:
template <typename T>
concept HighPrecisionSensor = requires {
    // make sure the type alias exists:
    typename T::value_type;

    // NESTED REQUIREMENT: Evaluates a compile-time boolean statement
    // Ensures the type can hold negative values AND handles high-precision numbers
    requires std::is_signed_v<typename T::value_type>;
    requires sizeof(typename T::value_type) >= 8;
};

// PASSES: double is signed and takes 8 bytes of space
struct RegThermostat {
    using value_type = double;
    double current_temperature;
};

// Passes: long double is >= 8 bytes. (usually 16 bytes on Linux x86_64)
struct PremiumThermostat {
    using value_type = long double;
    long double current_temp;
};

// FAILS: float takes up only 4 bytes of space
struct EcoThermostat {
    using value_type = float;
    float current_temperature;
};

// FAILS: unsigned int takes 4 bytes and cannot represent negative temperatures
struct BasicCounter {
    using value_type = unsigned int;
    unsigned int hardware_ticks;
};

// Use the concept in a function
void calibrate_device(HighPrecisionSensor auto sensor) {
    std::cout << "Successfully calibrated high-precision hardware.\n";
}

int main() {
    RegThermostat reg;
    PremiumThermostat premium;
    EcoThermostat eco;
    BasicCounter counter;

    calibrate_device(reg); // Valid!    
    calibrate_device(premium); // Valid!

    // --- COMPILER ERRORS (If uncommented) ---
    // calibrate_device(eco);     // Error: Fails size check (4 bytes)
    // calibrate_device(counter); // Error: Fails signed check and size check

    return 0;
}