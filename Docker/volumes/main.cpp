#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <regex>

namespace fs = std::filesystem;

int main() {
    std::string directory = "cpp-app-volume";
    int fileCount = 0;
    std::regex dataFilePattern("data\\d+\\.txt");

    // Ensure the directory exists
    if (!fs::exists(directory)) {
        fs::create_directory(directory);
    }

    // Iterate through the files in the directory using regular expressions
    for (const auto& entry : fs::directory_iterator(directory)) {
        std::string filename = entry.path().filename().string();
        if (std::regex_match(filename, dataFilePattern)) {
            fileCount++;
        }
    }

    // Create a new file for the current run
    std::string newFilename = directory + "/data" + std::to_string(fileCount + 1) + ".txt";
    std::ofstream outfile(newFilename);
    outfile << "This is run number " << (fileCount + 1) << std::endl;
    outfile.close();

    std::cout << "Created file: " << newFilename << std::endl;

    return 0;
}
