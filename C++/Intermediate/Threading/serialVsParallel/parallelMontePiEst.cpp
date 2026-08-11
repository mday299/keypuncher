#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <vector>

// Worker function for each thread
void monteCarloWorker(long long samples, long long& insideCircle, unsigned int seed) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    long long localCount = 0;
    for (long long i = 0; i < samples; ++i) {
        double x = dist(rng);
        double y = dist(rng);
        if (x*x + y*y <= 1.0) {
            localCount++;
        }
    }
    insideCircle = localCount;
}

int main() {
    const long long numSamples = 100000000LL; // 100 million samples
    unsigned numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4; // fallback

    std::vector<std::thread> threads;
    std::vector<long long> counts(numThreads, 0);

    long long samplesPerThread = numSamples / numThreads;

    auto start = std::chrono::high_resolution_clock::now();

    // Launch threads
    for (unsigned t = 0; t < numThreads; ++t) {
        threads.emplace_back(monteCarloWorker, samplesPerThread,
                             std::ref(counts[t]), 42 + t); // different seeds
    }

    // Join threads
    for (auto& th : threads) {
        th.join();
    }

    // Aggregate results
    long long insideCircle = 0;
    for (auto c : counts) insideCircle += c;

    double piEstimate = 4.0 * insideCircle / (samplesPerThread * numThreads);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Parallel Monte Carlo π estimate: " << piEstimate << "\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    return 0;
}
