#include <iostream>
#include <fstream>

int main() {
    std::ofstream outFile("output.txt");

    if (!outFile) {
        std::cerr << "Error opening file for writing" << std::endl;
        return 1;
    }

    std::string content = "Hello, World!\n";
    outFile << content;

    outFile.close();
    std::cout << "File written successfully" << std::endl;

    return 0;
}
