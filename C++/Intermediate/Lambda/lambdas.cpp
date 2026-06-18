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

Write a program that hs a lambda that contains two variables, one passed by 
 reference and one passed by value. Use the mutable expression to modify the 
 one passed by value into the lambda.

Write a program that uses std::functon to bind the lambda to a functor.
*/

#include <iostream> 
#include <vector>
#include <algorithm>
#include <functional>

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

    return 0;
}
