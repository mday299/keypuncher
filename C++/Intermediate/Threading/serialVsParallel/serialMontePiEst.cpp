#include <iostream>
#include <random>
#include <chrono>

int main() {
    const long long numSamples = 100000000LL; // 100 million samples
    long long insideCircle = 0;

    std::mt19937_64 rng(42); // fixed seed for reproducibility
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    auto start = std::chrono::high_resolution_clock::now();

    for (long long i = 0; i < numSamples; ++i) {
        double x = dist(rng);
        double y = dist(rng);
        if (x*x + y*y <= 1.0) {
            insideCircle++;
        }
    }

    double piEstimate = 4.0 * insideCircle / numSamples;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Serial Monte Carlo π estimate: " << piEstimate << "\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    return 0;
}
