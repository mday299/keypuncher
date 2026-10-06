#include <iostream>
#include <type_traits> // std::is_integral_v and std::is_signed_v
#include <cmath>       // Required for std::fmod

//Define the concept:
template <typename U>
//keyword  name  assign   returns true for an integral type (int, char, bool, etc.) 
                            //and false otherwise
concept integral   =        std::is_integral_v<U>;

//Indepdent check for signed
template <typename U>
concept signed_type = std::is_signed_v<U>;

//Combined both concepts to create a SignedInt:
template <typename U>
concept signed_int = integral<U> && signed_type<U>;

//Use the integral concept directly in the function parameters
void print_even_status(integral auto value) {
    if (value % 2 == 0){
        std::cout << value << " is even\n";
    } else {
        std::cout << value << " is odd\n";
    }
}

// Accepts ANY signed type (int, float, double, etc.)
void print_signed_even_status(signed_type auto value) {
    // std::fmod returns 0.0 if there is no remainder
    if (std::fmod(value, 2.0) == 0.0) {
        std::cout << value << " is a signed even number\n";
    } else {
        std::cout << value << " is a signed odd number\n";
    }
}

// Accepts ONLY signed int
void print_signed_int_status(signed_int auto value) {
    if (value % 2 == 0){
        std::cout << value << " is even\n";
    } else {
        std::cout << value << " is odd\n";
    }
}

int main() {
    // Inegral Checks
    print_even_status(6);  //Valid. int is integral
    print_even_status('A');  //Valid. 65 is valid
    print_even_status(true);  //Valid.
    print_even_status(false);  //Valid.    
    //print_even_status(12.3);  //Invalid. double (compiler error)

    // Signed checks
    print_signed_even_status(5); //Valid. int is signed
    print_signed_even_status(7.7f); //Valid. float is signed
    print_signed_even_status('D'); //Valid. char is signed and 68 is valid
    //print_signed_even_status(5u);  //Invalid. u means unsigned
    //print_signed_even_status(true); //Invalid. bool type is not signed
    
    // Signed int checks:
    print_signed_int_status(3);   //Valid.
    print_signed_int_status('E'); //Valid. 69 is valid
    //print_signed_int_status(93.2); //Invalid doule is not int
    
    return 0;
}
   