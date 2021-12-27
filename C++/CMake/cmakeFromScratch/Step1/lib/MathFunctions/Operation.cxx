#include <exception>
#include <stdexcept>
#include <iostream>
#include "Operation.hpp"

int Operation::sum(const int &a, const int &b){
    return a + b;
}
int Operation::mult(const int &a, const int &b){
    return a * b;
}
int Operation::div(const int &a, const int &b){
    if(b == 0){
        throw std::overflow_error("Divide by zero exception");
    }
    return a/b;
}
int Operation::sub(const int &a, const int &b){
    return a - b;
}
