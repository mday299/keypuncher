/* Template & Lambda Challenge Set (C++17)

See:
https://caiorss.github.io/C-Cpp-Notes/Libraries-and-featuresCPP17.html

https://gieseanw.wordpress.com/2017/05/03/a-true-heterogeneous-container-in-c/

2. Type‑safe heterogeneous container

Implement a container that stores arbitrary types but enforces compile‑time 
constraints on what can be inserted.

Core idea:  
A hetero_list<Ts...> that behaves like a type‑indexed tuple but supports runtime
 push/pop with template dispatch.

Requirements:

    Use a variant of std::variant or your own tagged union.

    Insertions must be rejected at compile time if the type doesn’t satisfy a
     trait (e.g., is_copy_constructible).

    Provide a visit_all using a lambda that is invoked for each stored element.

Extensions:

    Add a compile‑time “allowed types” policy.

    Add a lambda‑based transform that returns a new hetero_list with 
    transformed types.
*/

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <functional>

// Generic heterogeneous container class template
// ----------------------------------------------------------------------------
// Accepts a parameter pack of allowed types (e.g., int, double, std::string).
template<typename... Types>
class heterogeneous_container {
public:
    // Define the variant type that can hold any of the allowed types.
    using VariantType = std::variant<Types...>;

    // Add an element of any allowed type into the container.
    // Uses perfect forwarding to avoid unnecessary copies.
    template<typename T>
    void push_back(T&& value) {
        items.emplace_back(std::forward<T>(value));
    }

    // Clear all elements from the container.
    void clear() { items.clear(); }

    // Return total number of elements (all types combined).
    size_t size() const { return items.size(); }

    // Count number of elements of a specific type.
    // Uses std::holds_alternative<T> to check the active type in the variant.
    template<typename T>
    size_t number_of() const {
        size_t count = 0;
        for (auto& elem : items) {
            if (std::holds_alternative<T>(elem)) {
                ++count;
            }
        }
        return count;
    }

    // Visit all elements with a single visitor.
    // std::visit dispatches to the correct overload of the visitor.
    template<typename Visitor>
    void visit_all(Visitor&& visitor) {
        for (auto& elem : items) {
            std::visit(visitor, elem);
        }
    }

    // Chain multiple visitors in one pass.
    // Each visitor is applied sequentially to every element.
    // Uses a fold expression to expand over the visitor pack.
    template<typename... Visitors>
    void chain_visit_all(Visitors&&... visitors) {
        for (auto& elem : items) {
            (..., std::visit(visitors, elem));
        }
    }

private:
    // Internal storage: a vector of variants.
    // Each element can be one of the allowed types.
    std::vector<VariantType> items;
};

// ---------------- Example Visitors ----------------

// Print visitor: prints any type stored in the container.
// Uses a templated operator() to handle all types generically.
struct print_visitor {
    template<typename T>
    void operator()(const T& value) const {
        std::cout << value << " ";
    }
};

// Double numeric visitor: doubles ints and doubles.
// Ignores other types by providing a templated no-op overload.
struct double_numeric_visitor {
    void operator()(int& i) const { i += i; }
    void operator()(double& d) const { d += d; }
    template<typename T>
    void operator()(T&) const {} // ignore non-numeric types
};

// String append visitor: appends "bar" to strings.
// Ignores other types similarly.
struct string_append_visitor {
    void operator()(std::string& s) const { s += "bar"; }
    template<typename T>
    void operator()(T&) const {} // ignore non-string types
};

int main() {
    // Instantiate a heterogeneous container that can hold int, double, char, and std::string.
    heterogeneous_container<int, double, char, std::string> c;

    // Push heterogeneous values into the container.
    c.push_back('a');
    c.push_back(1);
    c.push_back(2.0);
    c.push_back(3);
    c.push_back(std::string{"foo"});

    // Print the container contents.
    std::cout << "Original c: ";
    c.visit_all(print_visitor{});
    std::cout << "\n";

    // Apply visitor to double ints and doubles.
    std::cout << "Visiting c (double ints and doubles)\n";
    c.visit_all(double_numeric_visitor{});
    std::cout << "c: ";
    c.visit_all(print_visitor{});
    std::cout << "\n";

    // Apply visitor to append "bar" to strings.
    std::cout << "Visiting c again (append \"bar\" to strings)\n";
    c.visit_all(string_append_visitor{});
    std::cout << "c: ";
    c.visit_all(print_visitor{});
    std::cout << "\n";

    // Lambda visitor to square numeric elements.
    // Uses if constexpr with std::is_same_v to check type at compile time.
    auto square_numeric = [](auto& value) {
        if constexpr (std::is_same_v<decltype(value), int&> ||
                      std::is_same_v<decltype(value), double&>) {
            value = value * value;
        }
    };

    // Apply chained visitors: double, then square, then append.
    std::cout << "Applying chained visitors (double, square, append)\n";
    c.chain_visit_all(double_numeric_visitor{}, square_numeric, string_append_visitor{});

    // Print after chaining.
    std::cout << "c after chaining: ";
    c.visit_all(print_visitor{});
    std::cout << "\n";

    // Show size and count of specific type.
    std::cout << "Size of c: " << c.size() << "\n";
    std::cout << "Number of integers in c: " << c.number_of<int>() << "\n";
}
