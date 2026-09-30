#include <iostream>
#include <type_traits> // std::is_integral

//Define the concept:
template <class T>
//keyword  name  assign   returns true for an integral type (int, char, bool, etc.) 
                            //and false otherwise
concept integral   =        std::is_integral_v<T>;

//Use the concept directly in the function parameters
void print_even_status(integral auto value) {
    if (value % 2 == 0){
        std::cout << value << " is even\n";
    } else {
        std::cout << value << " is odd\n";
    }
}

int main() {
    print_even_status(6);  //Valid. int is integral
    print_even_status('A');  //Valid.
    print_even_status(true);  //Valid.
    print_even_status(false);  //Valid.
    
    //print_even_status(12.3);  //Invalid. double (compiler error)
    
    return 0;
}
   