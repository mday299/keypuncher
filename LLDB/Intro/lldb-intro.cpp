#include <iostream>
#include <chrono>
#include <thread>

void arrUpdate(double* arr, unsigned int index, double val) {
    arr[index] = val;
}

int main() {
    double arr[5] = {1.0, 4.0, 12.0, 33.5, 101.8};

    arrUpdate(arr, 2, 332.4);

    for(auto num : arr) {
        std::cout << num << std::endl;
    }

    arrUpdate(arr, 4, 77.7);

//    while(true) {
//        std::cout << "Looping...\n";
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }

    std::cout << "Done.\n";
    return 0;
}