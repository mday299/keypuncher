#include <numeric>  //std::accumulate
#include <vector>   //std::vector
#include <concepts> //std::integral and std::floatng_point
#include <iostream> 

template <class T>
//only accept integral and floating point types
//Integral types include but are not limited to:
//bool, char, short, int, long, long long, wchar_t char32_t
//Note that it probably doesn't make semantic sense to do 
// bools and chars as Avereagable
requires std::integral<T> || std::floating_point<T>
//Averageable acccepts const vector types accepted 
//  (provided they are integral or floating point)
//& means pass by reference
constexpr double Averageable(std::vector<T> const &vec) {
    const double sum = std::accumulate(vec.begin(), vec.end(),
          //double explicitly specified! 
          0.0);
          //Specifically avoiding long double because it can be implementation-dependent
          //on many x86-64 Linux it provides 80-bit extended precision but
          //C++ does NOT require it!
    
    return sum / vec.size();
}

int main() {
    std::vector ints {1, 2, 3, 4, 5};
    std::cout << Averageable(ints) << "\n";

    std::vector floats {3.0f, 6.7f, 9.0f};
    std::cout << Averageable(floats) << "\n";

    std::vector doubles {7.0, 1.0, 5.0};
    std::cout << Averageable(doubles) << "\n";
    
    // Massive compiler error
    //std::vector mixed {1, 2.2f, 5.0};
    //std::cout << Averageable(mixed) << "\n";

    return 0;
}
   