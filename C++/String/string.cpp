/* 
Write a program that defines two strings. 
Join them together and assign the result to a third string. 
Print out the value of the resulting string.

Write a program that accepts the first and the last name
 from the standard input using the std::getline function. 
Store the input in a single string called fullname. 
Print out the string.

Write a program that creates two substrings from the main string.
The main string is made up of first and last names and is equal to
 “John Doe.” The first substring is the first name. 
 The second substring is the last name. 
Print the main string and two substrings afterward.

Write a program that defines the main string with a value of 
“Hello C++ World.” and checks if a single character ‘C’ is found
 in the main string.

 Write a program that defines the main string with a value of
  “Hello C++ World.” and checks if a substring “C++” is found
   in the main string.
*/

#include <iostream>
#include <string>

int main() {
    //part 1
    std::string howdy = "Howdy ";
    std::string buddy = "Buddy";
    std::string joined = howdy + buddy;
    
    std::cout << joined << std::endl;

    //part 2
    std::string fullName;
    std::cout << "Please enter your full name: ";
    std::getline(std::cin, fullName);
    std::cout << "Name entered: " << fullName << std::endl;

    //part 3
    fullName = "John Doe";
    std::string firstName = fullName.substr(0, 4);
    std::string lastName = fullName.substr(5, 3);
    std::cout << "Name: " << fullName << std::endl;
    std::cout << "First: " << firstName << std::endl;
    std::cout << "Last: " << lastName << std::endl;

    //part 4
    std::string hello = "Hello C++ World.";
    char C = 'C';
    auto iterator = hello.find(C);
    if (iterator != std::string::npos) {
        std::cout << "Character found at position: " << iterator << std::endl; 
    } else {
        std::cout << "Character not found." << std::endl;
    }

    //part 5
    std::string cPlusPlus = "C++";
    iterator = hello.find(cPlusPlus);
    if (iterator != std::string::npos) {
        std::cout << "Substring found at position: " << iterator << std::endl; 
    } else {
        std::cout << "Substring not found." << std::endl;
    }

    return 0;
}