#include <iostream>
#include <thread>

int main() {
    int n = 3;
    
    // Create a thread that runs 
    // a lambda expression
    std::thread t([](int n){
        std::cout << n << std::endl;
    }, n);

    // Wait for the thread to complete
    t.join();
    return 0;
}