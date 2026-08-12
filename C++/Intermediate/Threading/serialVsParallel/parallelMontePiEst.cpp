#include <iostream>   // For console output (std::cout)
#include <random>     // For random number generation (std::mt19937_64, distributions)
#include <chrono>     // For timing execution
#include <thread>     // For multithreading (std::thread)
#include <vector>     // For dynamic arrays (std::vector)

// ---------------------------------------------------------------
// monteCarloWorker (worker function per thread)
// ---------------------------------------------------------------
// Each thread runs this function to perform Monte Carlo sampling.
// It generates random points and counts how many fall inside the unit circle.
//
// Parameters:
//   samples: number of random points this thread should generate
//   insideCircle: reference to thread-local counter (will be written back)
//   seed: unique seed for RNG to ensure independent random streams
// ---------------------------------------------------------------
void monteCarloWorker(long long samples, long long& insideCircle, unsigned int seed) {
    // Initialize random number generator with given seed
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    long long localCount = 0; // local counter for points inside circle

    // Generate 'samples' (random points)
    for (long long i = 0; i < samples; ++i) {
        double x = dist(rng);  // random x in [0,1]
        double y = dist(rng);  // random y in [0,1]
        if (x*x + y*y <= 1.0) {
            localCount++;      // point lies inside quarter-circle
        }
    }
    // Write result back to shared vector slot
    insideCircle = localCount;
}

int main() {
    const long long numSamples = 100000000LL; // 100 million samples

    // -----------------------------------------------------------
    // Determine number of threads to use
    // std::thread::hardware_concurrency() queries available cores
    // Fallback to 4 threads if query fails
    // -----------------------------------------------------------    
    unsigned numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4; // fallback

    std::cout << "Number of threads launched: " << numThreads << "\n";

    // Thread container and per-thread counts
    std::vector<std::thread> threads;
    std::vector<long long> counts(numThreads, 0);

    // Divide total samples evenly among threads
    long long samplesPerThread = numSamples / numThreads;

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Launch threads
    // -----------------------------------------------------------
    // Each thread runs monteCarloWorker with:
    //   - samplesPerThread samples
    //   - reference to its own slot in 'counts'
    //   - unique seed (42 + thread index)
    // -----------------------------------------------------------
    for (unsigned t = 0; t < numThreads; ++t) {
        threads.emplace_back(monteCarloWorker, samplesPerThread,
                             std::ref(counts[t]), // pass reference so worker
                                                  // can write result
                             42 + t); // different seeds per thread
    }

    // Join threads:  Wait for all threads to finish before aggregating results
    for (auto& th : threads) {
        th.join();
    }

    // Aggregate results
    // -----------------------------------------------------------
    // Sum up counts from all threads to get total points inside circle
    // -----------------------------------------------------------
    long long insideCircle = 0;
    for (auto c : counts) insideCircle += c;

    // Compute Pi estimate
    double piEstimate = 4.0 * insideCircle / (samplesPerThread * numThreads);

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Parallel Monte Carlo Pi estimate: " << piEstimate << "\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    return 0;
}
