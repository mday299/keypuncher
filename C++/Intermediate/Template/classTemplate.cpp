/*
Class Template
Write a program that defines a simple class template with one data member of a
 generic type, a constructor, a getter function of a generic type, and a setter 
 member function. 
Instantiate the class template in the main function using types int and double.
Then try the same with std::string.
Then try the same with std::vector
*/

#include <iostream>
#include <string>
#include <vector>

template <typename T>
class MyClass {
private:
    T x;
public:
    MyClass(T xx)
        : x{xx} {}

    T getx() const {
        return x;
    }
    void setx(T ax) {
        x = ax;
    }
};

int main() {
    MyClass<int> o{123};
    std::cout << "Data member value is: " << o.getx() << std::endl;
    o.setx(789);
    std::cout << "Data member value is: " << o.getx() << std::endl;

    MyClass<double> o2{4.56};
    std::cout << "Data member value is: " << o2.getx() << std::endl;
    o2.setx(555.3);
    std::cout << "Data member value is: " << o2.getx() << std::endl;

    MyClass<std::string> o3{"foo"};
    std::cout << "Data member value is: " << o3.getx() << std::endl;
    o3.setx("bar");
    std::cout << "Data member value is: " << o3.getx() << std::endl;

    MyClass<std::vector<int>> o4(std::vector<int>{76, 88});
    //std::cout << "Data member value is: " << o4.getx() << std::endl;
    //doesn't work because there isn't an ostream associated with a
    // vector of ints. Use this instead:
    auto v = o4.getx();
    std::cout << "Data member value is: ";
    for (int n : v)
        std::cout << n << ' ';
    std::cout << std::endl;
   
    return 0;
}