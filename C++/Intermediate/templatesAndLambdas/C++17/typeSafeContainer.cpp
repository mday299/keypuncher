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
#include <chrono>
#include <string>
#include <typeinfo>

#include <any>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

std::string demangle(const char* name) {
#ifdef __GNUG__
    int status = 0;
    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    std::string result = (status == 0 && demangled) ? demangled : name;
    std::free(demangled);
    return result;
#else
    return name; // fallback
#endif
}

struct Point{
    double x;
    double y;

    Point(double x, double y): x(x), y(y) {}

    Point(const Point& p){
        std::cerr << " -->> Copy constructor" << '\n';
        x = p.x;
        y = p.y;
    }
};

template<typename T>
auto printInfo(const std::any& x) -> void {
    std::cout << " x.type = " << demangle(x.type().name())
              << " ; value(x) = "
              << std::any_cast<const T&>(x)
              << '\n';
}

std::ostream& operator<<(std::ostream& os, const Point& p){
    os << "Point(" << p.x << ", " << p.y << ") ";
    return os;
}

int main() {
    // Print boolean as 'true', 'false', instead of 0 or 1
    std::cout << std::boolalpha;

    // 2. Type-safe heterogenous container

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    //First implement a generic container with std::any (based on boost:any)
    std::any x;
    x.emplace<Point>(100.0, 20.0);
    printInfo<Point>(x);
    std::cout << "Has value: x.has_value() = " << x.has_value() << '\n';

    std::cout << "Try valid casting " << std::endl;
    try{
        std::any_cast<Point>(x);
    } catch (const std::bad_any_cast& ex) {
        std::cerr << " >>> Exception: what = " << ex.what() << '\n'; 
    }
    std::cout << "Done with valid casting \n" << std::endl;

    std::cout << "Try after reset() casting " << std::endl; 
    x.reset();
    x = "testing type casting";                                                        
    std::cout << "Has value: x.has_value() = " << x.has_value() << '\n';
    try{
        std::any_cast<int>(x);
    } catch (const std::bad_any_cast& ex) {
        std::cerr << " >>> Exception: what = " << ex.what() << '\n'; 
    }
    std::cout << "Done with invalid casting after reset\n" << std::endl;

    std::cout << "Try valid cast on p " << std::endl; 
    std::any p;
    p.emplace<Point>(3100.0, -70.0);
    printInfo<Point>(p);
    std::cout << "Has value: p.has_value() = " << p.has_value() << '\n';  
    try{
        std::any_cast<Point>(p);
    } catch (const std::bad_any_cast& ex) {
        std::cerr << " >>> Exception: what = " << ex.what() << '\n'; 
    }
    std::cout << "Done with second valid cast\n" << std::endl;

    std::cerr << " >>> End the program gracefully" << '\n'; 

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    return 0;
}