/*
Write a program that automatically deduces the type for char, int,
 and double objects based on the initializer used. 
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

    std::cout << character << ", " << integer << ", " << dbl << std::endl;
    
    std::cout << typeid(character).name() << std::endl;
    std::cout << typeid(integer).name() << std::endl;
    std::cout << typeid(dbl).name() << std::endl;

    return 0;
}