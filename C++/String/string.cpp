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

    return 0;
}