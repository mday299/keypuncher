#include <iostream>             // Standard C++ I/O
#include <thread>               // std::thread for concurrency
#include <queue>                // std::queue for message buffers
#include <mutex>                // std::mutex for synchronization
#include <condition_variable>   // std::condition_variable for signaling
#include <string>               // std::string for message content
#include <chrono>               // std::chrono for timing
#include <cstdlib>              // rand() for random delays
#include <semaphore>            // C++20 standard counting_semaphore

// Shared queues between threads
std::queue<std::string> rawQueue;       // Holds input messages before processing
std::queue<std::string> processedQueue; // Holds processed messages before output

// Synchronization primitives
std::mutex mtx;              // Protects access to queues and flags
std::condition_variable cv;  // Signals when new work is available
bool inputDone = false;      // Flag: input thread finished
bool processingDone = false; // Flag: all processing threads finished
int activeProcessors = 0;    // Count of currently active processing threads

// Critical section variable
int criticalCounter = 0;    // Shared counter incremented in critical section
std::mutex criticalMtx;     // Protects criticalCounter

// Semaphore guarding arbitrary limited resource (capacity = 2)
std::counting_semaphore<2> resourceSem(2);

// -------------------- Input Thread --------------------
void inputThread() {
    for (int i = 0; i < 10; ++i) {
        {
            // Lock mutex before pushing into rawQueue
            std::lock_guard<std::mutex> lock(mtx);
            rawQueue.push("Message " + std::to_string(i));
        }
        // Notify processors that new data is available
        cv.notify_all();
        
        // Random stagger to simulate irregular input arrival
        std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 400));
    }
    // After all messages are pushed, mark input as done
    std::lock_guard<std::mutex> lock(mtx);
    inputDone = true;
    cv.notify_all();
}

// -------------------- Processing Thread --------------------
void processingThread(int id) {
    {
        // Increment active processor count when thread starts
        std::lock_guard<std::mutex> lock(mtx);
        activeProcessors++;
    }
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait until rawQueue has data or input is finished
        cv.wait(lock, [] { return !rawQueue.empty() || inputDone; });

        if (!rawQueue.empty()) {
            // Pop one message from rawQueue
            std::string msg = rawQueue.front();
            rawQueue.pop();
            lock.unlock(); // Release lock while processing

            // Acquire semaphore before using resource
            resourceSem.acquire();

            // Critical section: increment shared counter
            {
                std::lock_guard<std::mutex> critLock(criticalMtx);
                criticalCounter++;
                std::cout << "Thread " << id
                          << " entered critical section. Counter = "
                          << criticalCounter << std::endl;
                // Random jitter inside critical section
                std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 200));
            }

            // Simulate heavy processing with random delay
            std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 1500));
            msg = "[Processed by " + std::to_string(id) + "] " + msg;

            // Push processed message into processedQueue
            std::lock_guard<std::mutex> lock(mtx);
            processedQueue.push(msg);
            cv.notify_all();

            // Release semaphore after resource use
            resourceSem.release();

        } else if (inputDone) {
            break;
        }
    }
    {
        // Decrement active processor count when thread finishes        
        std::lock_guard<std::mutex> lock(mtx);
        activeProcessors--;
        // If all processors are done, mark processingDone
        if (activeProcessors == 0) {
            processingDone = true;
            cv.notify_all();
        }
    }
}

// -------------------- Output Thread --------------------
void outputThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait until processedQueue has data or processing is finished
        cv.wait(lock, [] { return !processedQueue.empty() || processingDone; });

        if (!processedQueue.empty()) {
            // Pop one processed message
            std::string msg = processedQueue.front();
            processedQueue.pop();
            lock.unlock();

            // random delay before printing to simulate output jitter
            std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 1000));
            std::cout << msg << std::endl;
        } else if (processingDone) {
            // Exit loop if all processing threads are finished            
            break;
        }
    }
}

int main() {
    srand(time(nullptr)); // Seed random number generator

    // Launch threads    
    std::thread t1(inputThread);
    std::thread t2(processingThread, 1);
    std::thread t3(processingThread, 2);
    std::thread t4(processingThread, 3);
    std::thread t5(processingThread, 4);
    std::thread t6(outputThread);

    // Wait for all threads to finish    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    // Print final critical counter value
    std::cout << "Final criticalCounter = " << criticalCounter << std::endl;
    return 0;
}
