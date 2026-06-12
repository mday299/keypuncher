/*
Function Template
Write a program that defines a template for a function that sums two numbers.
 Numbers are of the same generic type, T, and are passed to the function as 
 arguments. 
Instantiate the function in the main program using types int and double.
Also see what happens when you try std::string (it works)
You won't be able to use std::vector because it doesn't define an addition operator.
*/

#include <iostream>
#include <string>
#include <vector>

template <typename T>
T mySum(T x, T y) {
    return x + y;
}

int main() {
    int intresult = mySum<int>(1, 3);
    double dblres = mySum<double>(1.6, 3.7);

    std::cout << intresult << std::endl;
    std::cout << dblres << std::endl;

    //try a string
    std::string strResult = mySum<std::string>("a", "b");
    std::cout << strResult << std::endl;

    //try a vector
    //std::vector vecReult = mySum<std::vector>(1, 2); //compiler error!

    return 0;
}