/*
Write a program that defines an object of type double. 
Define a pointer that points to that object. Print the
 value of the pointed-to object by dereferencing a pointer.

 Write a program that defines an object of type double
  called mydouble. Define an object of reference type 
  called myreference and initialize it with mydouble. 
  Change the value of myreference. Print the object value
   using both the reference and the original variable. 
  Change the value of mydouble. Print the value of both objects.
*/

#include <iostream>
int main () {
    //Part 1
    double d = 3.0;
    double *p = &d;

    std::cout << *p << std::endl;

    //Part 2
    double myDouble = 36.2;
    double &myReference = myDouble;

    myReference = 7.1;
    std::cout << myReference << ", " << myDouble << std::endl;

    myDouble = 44.4;
    std::cout << myReference << ", " << myDouble << std::endl;

    return 0;
}