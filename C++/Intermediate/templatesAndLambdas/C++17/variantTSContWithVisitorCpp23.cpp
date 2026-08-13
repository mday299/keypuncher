#include <iostream>
#include <string>
#include <variant>
#include <array>
#include <concepts>

// A fixed-size heterogeneous container that works in constexpr contexts.
// - N is the number of elements.
// - Types... is the list of allowed types stored in the variant.
template<std::size_t N, typename... Types>
struct constexpr_container {
    using VariantType = std::variant<Types...>;   // Each element can be one of these types.
    std::array<VariantType, N> items;             // Fixed-size array of variants.

    // Visit all elements with a single visitor.
    // Marked constexpr so it can be used at compile time.
    template<typename Visitor>
    constexpr void visit_all(Visitor&& visitor) const {
        for (auto& elem : items) {
            std::visit(visitor, elem);            // Dispatch to the correct overload/lambda.
        }
    }

    // Apply multiple visitors sequentially to each element.
    // Fold expression (..., std::visit(...)) expands over all visitors.
    template<typename... Visitors>
    constexpr void chain_visit_all(Visitors&&... visitors) {
        for (auto& elem : items) {
            (..., std::visit(visitors, elem));    // Apply each visitor in order.
        }
    }
};

int main() {
    // Build a constexpr container with 3 elements: int, double, char.
    // This is allowed at compile time because std::array does not allocate dynamically.
    constexpr constexpr_container<3, int, double, char> c {{
        2, 3.0, 'x'
    }};

    // Transform the container at compile time.
    // We pass c into the lambda as a parameter (no capture).
    constexpr auto transformed = [] (auto input) {
        auto tmp = input;   // Make a copy we can mutate.

        // Visitor #1: doubles numeric values.
        // Uses concepts (std::integral, std::floating_point) to constrain to numeric types.
        auto double_numeric = []<typename T>(T& value) {
            if constexpr (std::integral<T> || std::floating_point<T>)
                value += value;   // Double the value.
        };

        // Visitor #2: squares numeric values.
        // Same constraints, but performs squaring instead of doubling.
        auto square_numeric = []<typename T>(T& value) {
            if constexpr (std::integral<T> || std::floating_point<T>)
                value *= value;   // Square the value.
        };

        // Apply both visitors in sequence to every element.
        tmp.chain_visit_all(double_numeric, square_numeric);

        return tmp;   // Return the transformed container.
    }(c); // Pass c into the lambda.

    // Runtime visitor for printing values.
    auto print_visitor = []<typename T>(const T& value) {
        std::cout << value << " ";
    };

    // Print the transformed container at runtime.
    transformed.visit_all(print_visitor);
    std::cout << "\n";

    // Compile-time checks using static_assert.
    // These validate that the transformations happened correctly at compile time.
    static_assert(std::get<int>(transformed.items[0]) == 16);     // 2 doubled → 4, then squared → 16
    static_assert(std::get<double>(transformed.items[1]) == 36.0); // 3.0 doubled → 6.0, then squared → 36.0
}
