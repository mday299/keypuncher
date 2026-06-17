/*
Create an file stream that takes input from a files and parses strings with the
 std::getline() function.
Nest read from the same file but use char vice string.

Write to an output file using the insertion << operator.
Then write to the same file using the std::ios::app flag.
Then write to the same file using strings.

Now use a string stream to output an arbitrary string.
Then insert values of fundamental types into a string stream using the formatted
 output operator <<
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    //part 1
    std::fstream fstrm("myInFile.txt");
    std::string s;
    while(std::getline(fstrm, s)) {        
        std::cout << s << std::endl;
    }

    //reset file stream to beginning
    fstrm.clear();
    fstrm.seekg(0);
    //now read char vice string
    char c;
    std::cout << std::endl;
    while (fstrm >> std::noskipws >> c)
    {
        std::cout << c;
    }
    std::cout << std::endl;

    //part 2
    std::fstream outfs("myoutputfile.txt", std::ios::out);
    outfs << "First line of text." << std::endl;
    outfs << "Second line of text." << std::endl;

    std::fstream outfs2("myoutputfile.txt", std::ios::app);
    outfs2 << "Third line!" << std::endl;

    std::string s1 = "The first string\n";
    std::string s2 = "The second string\n";
    outfs2 << s1 << s2;
    std::cout << std::endl; 

    //part 3
    std::stringstream ss;
    ss << "Hello World." << std::endl;
    std::cout << ss.str();

    c = 'D';
    int x = 345;
    double d = 234.77;
    std::stringstream ss2;
    ss2 << c << ", " << x << ", " << d << std::endl;
    std::cout << ss2.str();

    return 0;
}