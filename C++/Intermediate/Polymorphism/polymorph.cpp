/*
Simple Polymorphism
Write a program that defines a base class with a pure virtual member function.
 Create a derived class that overrides a virtual function in the base class.
 Create a polymorphic object of a derived class through a unique pointer to a
  base class. 
Invoke the overridden member function through a unique pointer.

Polymorphism II
Write a program that defines a base class with a pure virtual member function.
 Derive two classes from the base class and override the virtual function 
 behavior. 
Create two unique pointers of base class type to objects of these derived
 classes. Use the pointers to invoke the proper polymorphic behavior.
*/

#include <iostream>
#include <memory>

class Base {
public:
    virtual void doWork() = 0;
    virtual ~Base() {} //make sure to clean up after!
};

class Derived : public Base {
public:
    void doWork() override {
        std::cout << "Do work from Derived" << std::endl;
    }
};

class Derived2 : public Base {
public:
    void doWork() override {
        std::cout << "Do work from SECOND Derived" << std::endl;
    }
};

int main(){
    //part 1
    std::unique_ptr<Base> obj = std::make_unique<Derived>();
    obj->doWork();

    //part 2
    std::unique_ptr<Base> obj2 = std::make_unique<Derived2>();
    obj2->doWork();

    return 0;
}