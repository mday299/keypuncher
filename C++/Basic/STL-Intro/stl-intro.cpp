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

Basic Set Operations
Write a program that defines a set of integers. Print out the set content and
 observe the following: the data is sorted, regardless of how we define the set.
Then insert two new values into the set. Then delete one arbitrary value.
 Print the resulting set.
Then search for data in the set using the find() member function. If found
 delete it. Print the resulting beginning and end sets.
Then use set member functions to check the set's size, determine if it is empty,
 and clear the set's contents.

Basic Map operations
Write a program that defines a map where keys are of type char and values are
 of type int. Print out the map’s content.
Then insert a value into te map using insert(). Then use the map's operator[] to
 inster another key-value element into the map. Print the map's content 
 afterwards.
The create another map with ints and strings. Print the map's content.
Search for and delete for 2 elements in both maps, one that is there,
 and one that is not.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <type_traits>
#include <set>
#include <map>

template <typename T>
struct ApproxEqual {
    T value;
    double eps; //epislon tolerance

    bool operator()(const T& other) const {
        if (std::is_floating_point<T>::value) {
            return std::fabs(other - value) < eps;
        } else {
            return other == value;
        }
    }
};

//clunky still. I need the iterator to perform the delete
template <typename Container, typename T>
bool contains(const Container& c, const T& value, double eps = 1e-9) {
    return std::find_if(c.begin(), c.end(),
                        ApproxEqual<T>{value, eps}) != c.end();
}

//Use when you need to get the iterator for deleting
template <typename Container, typename T>
auto find_approx(Container& c, const T& value, double eps = 1e-9) {
    return std::find_if(c.begin(), c.end(), ApproxEqual<T>{value, eps});
}

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

    std::cout << "dbls before delete: ";
    for (double d : dbls) {
        std::cout << d << ", ";
    }
    std::cout << std::endl;

    //there must be a better way to do this...
    /*
    if (std::find(dbls.begin(), dbls.end(), 55.7) == dbls.end())
        std::cout << "dbls does not contain " << 55.7 << '\n';
    else
        std::cout << "dbls contains " << 55.7 << '\n';

    if (std::find(dbls.begin(), dbls.end(), 55.6) == dbls.end())
        std::cout << "dbls does not contain " << 55.6 << '\n';
    else
        std::cout << "dbls contains " << 55.6 << '\n';   
    */

    //This is one way to do it better, but still not excellent
    /*
    if (contains(dbls, 55.7)) {
        std::cout << "dbls contain 55.7 deleting it\n";
        //how to delete??
    } else {
        std::cout << "dbls does not contain 55.7\n";
    }

    if (contains(dbls, 55.6)) {
        std::cout << "dbls does contain 55.6 deleting it\n";
        //how to delete?
    } else {
        std::cout << "dbls does not contain 55.6\n";
    }
    */

    auto it = find_approx(dbls, 55.7);

    if (it != dbls.end()) {
        std::cout << "dbls contains 55.7, deleting it\n";
        dbls.erase(it);
    } 

    it = find_approx(dbls, 55.6);

    if (it != dbls.end()) {
        std::cout << "dbls contains 55.6, deleting it\n";
        dbls.erase(it);
    }

    std::cout << "dbls after delete: ";
    for (double d : dbls) {
        std::cout << d << ", ";
    }
    std::cout << std::endl;

    //part 3
    std::set<int> intSet = {10, 3, -3, 9, 37, -8, 22, -50, 100};
    for (int i : intSet) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    intSet.insert(44);
    intSet.insert(44); //won't insert, duplicate value
    intSet.insert(-60);
    intSet.erase(3);
    intSet.erase(3); //won't redo the delete
    for (int i : intSet) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    //see last exericise for a better way to do this:
    if (std::find(intSet.begin(), intSet.end(), 37) == intSet.end()) {
        std::cout << "intSet does not contain " << 37 << '\n';
    } else {
        std::cout << "intSet contains " << 37 << ", deleting \n";
        intSet.erase(37);
    }
    if (std::find(intSet.begin(), intSet.end(), 38) == intSet.end()) {
        std::cout << "intSet does not contain " << 38 << '\n';
    } else {
        std::cout << "intSet contains " << 38 << ", deleting \n";
        intSet.erase(38);
    }
    for (int i : intSet) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    std::cout << intSet.size() << std::endl;
    intSet.clear();
    for (int i : intSet) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    //part 4
    std::map<char, int> map1 = { {'a', 1},{'b',5},{'c',10} };
    for (auto el : map1) {
        std::cout << el.first << ' ' << el.second << '\n';
    }
    std::cout << std::endl;

    map1.insert({'d', 15});
    map1['b'] = 20;
    for (const auto& el : map1) {
        std::cout << el.first << ' ' << el.second << '\n';
    }
    std::cout << std::endl;

    std::map<std::string, int> map2 = {{"red", 1}, {"green",5}, {"blue", 10} };
    for (const auto& el : map2) {
        std::cout << el.first << ' ' << el.second << '\n';
    }
    std::cout << std::endl;
    std::string findbykey = "blue";
    auto mapIt = map2.find(findbykey);
    if (mapIt != map2.end()) {
        std::cout << "Key found. Deleting the element..." << '\n';
        map2.erase(mapIt);
    } else {
        std::cout << "Key not found." << '\n';
    }

    findbykey = "yellow";
    mapIt = map2.find(findbykey);
    if (mapIt != map2.end()) {
        std::cout << "Key found. Deleting the element..." << '\n';
        map2.erase(mapIt);
    } else {
        std::cout << "Key not found." << '\n';
    }
    for (const auto& el : map2) {
        std::cout << el.first << ' ' << el.second << '\n';
    }
    std::cout << std::endl;

    return 0;
}