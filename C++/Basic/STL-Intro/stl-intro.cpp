/*
Basic Vectors
Write a program that defines a vector of integers. Insert two elements into the
 vector. Print out the vector content using the range-based for loop.
Then erase the 2nd element from the vector and print the vector again.
Then erase the first 3 elements from the vector.

Finding Elements in a Vector
Write a program that searches for a vector element using the std::find()
 algorithm function. If the element has been found, delete it. Print out the 
 vector content.
*/

#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    //part 1
    std::vector<int> ints = {1, 3, 5, 6};
    ints.push_back(2);
    ints.push_back(325);
    for (int i : ints) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    ints.erase(ints.begin() + 1); //erase 2nd element
    for (int i : ints) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    ints.erase(ints.begin(), ints.begin() + 3); //remove first 3 elements
    for (int i : ints) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    //part 2
    std::vector<double> dbls = {3.2, 1.46, 7.3, 2.0, 55.7};

    //there must be a better way to do this...
    if (std::find(dbls.begin(), dbls.end(), 55.7) == dbls.end())
        std::cout << "dbls does not contain " << 55.7 << '\n';
    else
        std::cout << "dbls contains " << 55.7 << '\n';

    if (std::find(dbls.begin(), dbls.end(), 55.6) == dbls.end())
        std::cout << "dbls does not contain " << 55.6 << '\n';
    else
        std::cout << "dbls contains " << 55.6 << '\n';   

    return 0;
}