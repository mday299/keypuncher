/* Write a program that defines and initializes an array
 of five doubles. Change and then print out the values of
  the first and last array elements. */

#include <iostream>

int main() {
    double arr[5] = {1.0, 4.0, 12.0, 33.5, 101.8};

    arr[0] = 44.6;
    arr[4] = 2;

    std::cout << arr[0] << ", " << arr[4] << std::endl;

    return 0;
}