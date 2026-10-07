
#include <concepts> //std::integral and std::floating_point
#include <type_traits> //std::common_type_t
#include <iostream> 

// ============================================================
// Average
// ============================================================
// Accepts any number of arguments as long as EVERY argument is
// either an integral type or a floating-point type.
//
// Examples:
//     Average(1, 2, 3)
//     Average(1, 2.5f, 3.0)
//     Average(1L, 2LL, 3.5)
//
// std::common_type_t<T...> determines the common type of all
// argument types, which is used here for the accumulation and
// therefore determines the type of the result.
// ============================================================

//Define a  template with a type parameter pack <class.... T>
template <class... T>
requires (
    //Require at least one type the type parameter pack.
    sizeof...(T) > 0 &&
    // For every T, evaluate the condition and && all results together.
    //  Therefore, every T must be integral or floating-point.
    ((std::integral<T> || std::floating_point<T>) && ...)
)
constexpr auto Average(T... values) { //(T... values is a function parameter pack)
    // Determine the common type of all types in the type parameter pack.
    using Common = std::common_type_t<T...>;

    // Common{} creates an initial value of the common type.
    // The expression is a binary left fold using + to accumulate
    // all values into that common type.
    Common sum = (Common{} + ... + values);

    //sizeof...(values) = number of function arguments
    return sum / sizeof...(values);
}

int main() {
    // 1. All integers
    auto a = Average(1, 2, 3, 4, 5);
    std::cout << "Average(1, 2, 3, 4, 5) = " << a << '\n';  
              
    //Mixed int and double
    auto c = Average(1, 2.5, 5);
    std::cout << "Average(1, 2.5, 5) = " << c << '\n';

    //Mixed integral types
    short s = 10;
    long l = 20L;
    long long ll = 30LL;

    auto e = Average(s, l, ll);
    std::cout << "Average(short, long, long long) = " << e << '\n';

    //Mix all
    short value1 = 10;
    long value2 = 20L;
    long long value3 = 30LL;

    auto f = Average(1, 2.2f, value1, 5.0, value2, value3);
    std::cout << "Average(mixed numeric types) = " << f << '\n';

    //Show what common_type_t selected
    using Common = std::common_type_t<
            int, float, short, double, long, long long>;

    static_assert(std::same_as<Common, double>, 
                  "Common type should be double");

    // ----------------------------
    // These are INVALID:
    // ---------------------------

    // No arguments. sizeof...(T) > 0 prevents this.
    // Uncomment to get compile error.
    //auto invalid4 = Average();

    // std::string is not integral or floating-point.
    // Uncomment to get compile error.
    //auto invalid1 = Average(1, 2.0, std::string{"hello"});

    // A pointer is neither integral nor floating-point.
    // Uncomment to get compile error.
    //auto invalid2 = Average(1, 2, nullptr);

    // This is actually double, so this assertion FAILS:
    // Uncomment to get compile error.
    //static_assert(std::same_as<Common, float>,
    //             "Expected Common to be float");

    return 0;
}
   