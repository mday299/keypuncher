/*
Lambdas are most commonly used as small, inline callable objects—especially as
 predicates (boils some variable(s) down to a bool), callbacks, 
 transformations passed to algorithms, and asynchronous operations. 
In modern C++ (17/20/23/26), a lambda is just one kind of callable: a type that
 can be invoked using function‑call syntax. Other callables include function 
 objects (functors), function pointers, std::function, and objects with an 
 overloaded operator(). 
When the behavior is expected to grow in complexity, requires multiple 
 operations, or needs a maintainable structure, a named function object is often 
 the better design choice (rather than a lambda expression).
*/
/*
Lambda Expressions
Write a program that defines a vector of integers. Sort the vector in a
 _descending_ order using the std::sort function and a user-provided lambda 
 function as a predicate.

Write a program that defines a vector of integers. Use the std::count_if
 function and a user-provided lambda function to count only even numbers.

write a program that defines a local lambda expression that can capture and
 modify the variable defined inside the main() function

Write a program that has a lambda that contains two variables, one passed by 
 reference and one passed by value. Use the mutable expression to modify the 
 one passed by value into the lambda.

Write a program that uses std::functon to bind the lambda to a functor.

Write a program that defines an event handler that stores the number of clicks
 from a UI element.

Write a program that stores the ID of a task thread and the prints it. Be sure
 to be careful about std::cout is not thread safe for interleaved writes!


*/

#include <iostream> 
#include <vector>
#include <algorithm>
#include <functional>

class Button {
public:
    // Store any callable: lambda, functor, std::bind, etc.
    void onClick(std::function<void()> handler) {
        clickHandler = handler;
    }

    // Simulate a click event
    void click() {
        if (clickHandler) {
            clickHandler();   // invoke the callback
        }
    }

private:
    std::function<void()> clickHandler;
};

// neccessary for thread pool example
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

//Note this is NOT a lock-free Thread Queue!
class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads) 
        : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            //create worker threads. Capture 'this' so tasks, etc. are available

            //Worker main loop
            workers.emplace_back([this]() {
                //Each worker in this example loops forever
                while (true) {
                    //task is declared outside the main loop so execution
                    //occurs after unlocking the queue
                    std::function<void()> task;
                    {
                        //unique_lock is required because condition_variable::wait 
                        //needs a lock it can atomically release and reacquire.
                        std::unique_lock<std::mutex> lock(queueMutex);
                        //Wakes up when notify_one() or notify_all() is called.
                        condition.wait(lock, [this]() {
                            return stop || !tasks.empty();
                        });

                        //This is subtle and important:
    //If stop == true AND there are no tasks left, the worker exits the lambda → thread ends.
    //If stop == true but tasks still exist, workers continue draining tasks before exiting.
                        //This ensures a graceful shutdown.
                        if (stop && tasks.empty())
                            return;

                        //Pop the task
                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task(); // execute the lambda
                }
            });
        }
    }

    //1. Lock the queue.
    //2. Push the task into the queue.
    //3. Unlock the queue (end of scope).
    //4. Notify one sleeping worker.
    //
    //std::forward<F>(f) preserves lvalue/rvalue-ness.
    //notify_one() wakes exactly one worker, not all.
    template <typename F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();

        // Join all threads. Joining ensures:
        // The thread has fully terminated.
        // All memory writes done by the thread are visible to the joining thread.
        // No worker will ever run again.
        // No tasks are left unexecuted.
        for (auto& t : workers)
            t.join();
    }

    //deal with std::cout not be thread safe for interleaved writes:
    std::mutex coutMutex;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
};

int main() {
    //part 1
    std::vector<int> myInts = {15, 8, 4, 9, 1, 5, 12};

    std::sort(std::begin(myInts), std::end(myInts), 
                [](int x, int y) {return x > y;} );

    for (const auto& el : myInts) {
        std::cout << el << ", ";
    }
    std::cout << std::endl;

    //part 2
    int myCount = std::count_if(std::begin(myInts), std::end(myInts),
                    [](int x) {return x % 2 == 0;} );
    std::cout << "Num even is " << myCount << std::endl;

    //part 3
    int var = 6;
    std::cout << "Value of variable before is " << var << std::endl;
    //define a lambda named increment
    auto increment = [&var](){ var++; };
    increment();
    std::cout << "Value of variable after lambda call is " << var << std::endl;

    //part 4
    int m = 0; int n = 0;
    //this bent my brain a little when I first saw the syntax
    [&, n] (int a) mutable { m = ++n + a; }(4); 
    std::cout << m << std::endl << n << std::endl;

    //part 5
    int i = 3; int j = 5;
    //NOTE The following lambda expression captures i by val and j by reference
    std::function<int (void)> f = [i, &j] { return i + j; };
    //change the values of i and j:
    i = 12; j = 66;
    //call f and print the result
    std::cout << f() << std::endl;

    //[]() noexcept { throw 8; }(); //this will result in a compiler warning in g++
    // AND probably a core dump!

    //part 6
    //Event handler callbacks
    //Lambdas let you bind local state directly into the handler without 
    //creating a class.
    Button uiButton;
    int clickCount = 0;
    uiButton.onClick([&clickCount]() {
        ++clickCount;
        std::cout << "Button clicked " << clickCount << " times\n";
    });
    uiButton.click();   // triggers the lambda
    uiButton.click();   // triggers again

    //part 7
    ThreadPool threadPool(4);   // 4 worker threads
    int taskId = 42;

    threadPool.enqueue([&threadPool, taskId]() {
        std::lock_guard<std::mutex> lock(threadPool.coutMutex);
        std::cout << "Task " << taskId
                << " running on thread "
                << std::this_thread::get_id()
                << "\n";
    });

    threadPool.enqueue([&threadPool]() {
        std::lock_guard<std::mutex> lock(threadPool.coutMutex);
        std::cout << "Another task running"
            << " running on thread "
            << std::this_thread::get_id()
            << "\n";
    });

    return 0;
}
