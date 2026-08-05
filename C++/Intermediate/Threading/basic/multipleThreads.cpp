#include <iostream>  // Standard C++ I/O
#include <thread>    // std::thread class
#include <chrono>    // Time utilities like std::chrono:seconds

// Task function executed by thread t1
void task1() {
    int i = 0;
    // Print the thread ID (unique identifier assigned by the OS)
    std::cout << "Thread 1 is running. ID: " << std::this_thread::get_id() << "\n";

    //Loop 5 times
    while (i < 5) {
        //Pause execution of this thread for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // After waking up, print a message
        std::cout << "Delay finished!" << std::endl;
        // Be sure to increment i or you will loop forever
        i++;
    } 
}

// Task function executed by thread t2
void task2() {
    // Print the thread ID
    std::cout << "Thread 2 is running. ID: " << std::this_thread::get_id() << "\n";
}

int main() {
    // Create two threads: t1 runs task1, t2 runs task2
    std::thread t1(task1);
    std::thread t2(task2);

    // Print thread IDs from the main thread’s perspective
    std::cout << "t1 ID: " << t1.get_id() << "\n";
    std::cout << "t2 ID: " << t2.get_id() << "\n";

    // If t2 is joinable, detach it
    // Detach means t2 runs independently; main thread will not wait for it
    if (t2.joinable()) {
        t2.detach();

        //sleep in main thread for 10 seconds and make sure t1 gets along a ways
        std::this_thread::sleep_for(std::chrono::seconds(10));

        std::cout << "t2 detached\n";
    }

    // If t1 is joinable, join it
    // Join means main will block until t1 finishes its loop
    if (t1.joinable()) {
        t1.join();
        std::cout << "t1 joined\n";
    } else {
        std::cout << "There seems to be a problem. Thread is not joinable" <<
            std::endl;
    }

    // Sleep briefly to ensure detached thread output completes.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Main thread finished.\n";
    return 0;
}
