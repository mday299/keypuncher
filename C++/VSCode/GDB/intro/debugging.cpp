#include <iostream>
#include <string>

int loopInt (int iterations) {
    int i;
    for (i = 0; i < iterations; i++) {
        std::cout << i << std::endl;
    }
    return i;
}

int main (int argc, char *argv[]) { 
    std::cout << "Howdy" << std::endl;

    float theFloat = 8;
    std::string theString = "Boo!";
    int it;

    it = loopInt(5);

    return 0;
}