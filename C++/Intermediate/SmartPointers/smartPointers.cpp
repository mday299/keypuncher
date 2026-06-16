/*
A Simple Unique Pointer
Write a program that defines a unique pointer to an integer value. 
 Use the std::make_unique function to create the pointer.

Unique Pointer to an Object of a Class
Write a program that defines a class with two data members, a user-defined
 constructor, and one member function. 
Create a unique pointer to an object of the class. Use the smart pointer to
 access the member function.

Shared Pointer Exercise
Write a program that defines three shared pointers pointing at the same object
 of type int. Create the first pointer through a std::make_shared function. 
 Create the remaining pointers by copying the first pointer. Access the 
 pointed-to object through all the pointers. 
*/

#include <iostream>
#include <memory>

class MyClass {
public:
    int myInt;
    double myDouble;

    MyClass(int i, double d)
       : myInt(i), myDouble(d) {
       }

    void printValues() {
        std::cout << myInt << ", " << myDouble << std::endl;
    }

};

int main() {
    //part 1
    std::unique_ptr<int> pInt = std::make_unique<int>(345);
    std::cout << *pInt << std::endl;

    //part 2
    std::unique_ptr<MyClass> obj = std::make_unique<MyClass>(4, 562.3);
    obj->printValues();

    //part 3
    std::shared_ptr<int> pInt1 = std::make_shared<int>(123);
    std::shared_ptr<int> pInt2 = pInt1;
    std::shared_ptr<int> pInt3 = pInt2;

    std::cout << pInt1 << ", " << *pInt1 << std::endl;
    std::cout << pInt2 << ", " << *pInt2 << std::endl;
    std::cout << pInt3 << ", " << *pInt3 << std::endl;

    return 0;
}
