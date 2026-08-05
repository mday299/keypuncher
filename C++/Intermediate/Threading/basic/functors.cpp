#include <iostream>
#include <thread>

// Define a function object (functor)
class SumFunctor {
public:
    int n;
    SumFunctor(int a) : n(a) {}

    // Overload the operator() to 
    // make it callable
    void operator()() const {
        std::cout << n << std::endl;
    }
};

int main() {

    // Create a thread using 
    // the functor object
    std::thread t(SumFunctor(3));

    // Wait for the thread to 
    // complete
    t.join();
    return 0;
}