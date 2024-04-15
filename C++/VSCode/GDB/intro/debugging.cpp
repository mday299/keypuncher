#include <iostream>

int loopInt (int iterations) {
    int i;
    for (i = 0; i < iterations; i++) {
        std::cout << i << std::endl;
    }
}

int main (int argc, char *argv[]) { 
    std::cout << "Howdy" << std::endl;

    loopInt(5);

    return 0;
}