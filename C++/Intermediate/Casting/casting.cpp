/*
STAY AWAY FROM dynamic_cast and REALLY away from reinterpret_cast!

Why do we prefer implicit converstion with static_cast?
Implicit Conversion.
Create types char, int, double, boolean, and string.
See what happens with an implicit cast across several types.

Narrowing Conversion
Convert from an int to a double with an implicit conversion

Convert from and int to a pointer to a void * with implicit conversion.
Then do the same with an explict static_cast

Why prefer std::array and std::vector?
Define a single dimensional array with some initial values.
Define a pointer to the first element to the array. 

*/
#include <iostream>
#include <string>

int main() {
    //part 1
    char myChar = 65; //A
    int myInt = 345;
    double myDouble = 789.65;
    bool myBool = true;
    std::string myStr = "xyz";
    std::cout << myChar << ", " << myInt << ", " << myDouble << ", " <<
        myBool << " " << myStr << std::endl;

    //NOTE that the order this is done matters!
    myStr = myChar;
    myChar = myBool;    
    myDouble = myInt;
    myInt = myChar;
    myBool = !myBool;
    std::cout << myChar << ", " << myInt << ", " << myDouble << ", " <<
        myBool << " " << myStr << std::endl;

    //part 2
    myInt = 678;
    myDouble = 234.567;
    std::cout << myInt << ", " << myDouble << std::endl;

    myInt = myDouble;
    std::cout << myInt << ", " << myDouble << std::endl;

    //part 3
    myInt = 123;
    int* pInt = &myInt;
    void* pVoid = pInt; //convert from int to pointer
    std::cout << myInt << ", " << pInt << ", " << pVoid << std::endl;

    int* pInt2 = static_cast<int*>(pVoid);
    std::cout << pInt2 << std::endl; 
    std::cout << *pInt2 << std::endl; //dereferece pointer

    //part 4
    int arr[5] = { 1, 2, 3, 4, 5 };
    pInt = arr; //point to the first array element
    std::cout << pInt << ", " << *pInt << std::endl;

    return 0;
}