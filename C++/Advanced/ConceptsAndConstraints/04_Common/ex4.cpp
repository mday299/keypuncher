
#include <concepts> //std::integral and std::floatng_point
#include <type_traits> //std::common_type_t
#include <iostream> 

// ============================================================
// Averageable
// ============================================================
// Accepts any number of arguments as long as EVERY argument is
// either an integral type or a floating-point type.
//
// Examples:
//     Average(1, 2, 3)
//     Average(1, 2.5f, 3.0)
//     Average(1L, 2LL, 3.5)
//
// std::common_type_t<T...> determines the type used for the
// accumulation and final result.
// ============================================================
template <class... T>
requires (sizeof ... (T) > 0 &&
    (std::integral<T> || std::floating_point<T>))
constexpr auto Averageable(T... values) {
    using Common = std::common_type_t<T...>
    Common sum = (Common{} + ... + values);

    return sum / sizeof...(values);
}

int main() {
    // 1. All integers
    auto a = Averageable(1, 2, 3, 4, 5);
    std::cout << "Average(1, 2, 3, 4, 5) = "
              << a << '\n';    

    return 0;
}
   