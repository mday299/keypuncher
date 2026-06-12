/*
Static Variable
Write a program that checks how many times a function was called from the main
 program. We will use a local static variable inside a function, and we will
 increment this variable each time the function is called in main()

Static Data Member
Write a program that defines a class with one static data member of type
 std::string. Make the data member public. 
Then, define this static data member outside the class.
 Change the static data member’s value from the main function.

Static Member Function
Write a program that defines a class with one static member function and one
 regular, nonstatic member function. Make both functions public. Define both
 member functions outside the class. 
Invoke both functions from the main program

*/
#include <iostream>
#include <string>

void myfunction() {
    static int numCalls = 0; 
    numCalls++;
    std::cout << numCalls << std::endl;
}

class MyClass {
public:
    static std::string myStr;

    static void myStaticMemFunc();
    void myRegFunc();
};

void MyClass::myStaticMemFunc() {
    std::cout << "Inside myStaticMemFunc" << std::endl;
}

void MyClass::myRegFunc() {
    std::cout << "Inside myRegFunc" << std::endl;
}

std::string MyClass::myStr = "John Doe";

int main() {
    //part 1
    myfunction();
    myfunction();

    //part 2
    std::cout << MyClass::myStr << std::endl;
    MyClass::myStr = "lmno";
    std::cout << MyClass::myStr << std::endl;

    //part 3
    MyClass::myStaticMemFunc();
    MyClass myobj;
    myobj.myRegFunc();

    return 0;
}