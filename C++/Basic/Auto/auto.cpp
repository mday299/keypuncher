/*
Write a program that automatically deduces the type for char, int,
 double and string objects based on the initializer used. 
Print out the values afterward.
Then determine the type programmatically 
 WITHOUT forcing a compile time error 
 (not always possible with more complex types)

*/

#include <iostream>
#include <typeinfo>

int main() {
    auto character = 'f';
    auto integer = 2;
    auto dbl = 2.3;
    auto str = "boogie";

    std::cout << character << ", " << integer << ", " << dbl <<
        ", " << str << std::endl;
    
    std::cout << typeid(character).name() << std::endl;
    std::cout << typeid(integer).name() << std::endl;
    std::cout << typeid(dbl).name() << std::endl;
    std::cout << typeid(str).name() << std::endl;  

    return 0;
}