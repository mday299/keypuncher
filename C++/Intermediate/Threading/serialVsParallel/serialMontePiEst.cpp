#include <iostream>   // For console output (std::cout)
#include <random>     // For random number generation (std::mt19937_64, distributions)
#include <chrono>     // For timing execution

int main() {
    const long long numSamples = 100000000LL; // 100 million samples
    long long insideCircle = 0;     // Counter for points inside the unit circle

    // -----------------------------------------------------------
    // Random number generator setup
    // -----------------------------------------------------------
    // std::mt19937_64: 64-bit Mersenne Twister RNG (fast, high-quality)
    // Seed = 42 → fixed seed ensures reproducibility (same sequence each run)
    // Distribution: uniform real numbers in [0.0, 1.0)
    // -----------------------------------------------------------
    std::mt19937_64 rng(42); // fixed seed for reproducibility
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // -----------------------------------------------------------
    // Monte Carlo sampling loop
    // -----------------------------------------------------------
    // For each sample:
    //   1. Generate random (x,y) in [0,1] × [0,1]
    //   2. Check if point lies inside unit quarter-circle (x^2 + y^2 <= 1)
    //   3. Increment counter if inside
    //
    // Geometric interpretation:
    //   Area of quarter-circle of radius 1 = π/4
    //   Ratio (insideCircle / numSamples) ≈ π/4
    //   Multiply by 4 → estimate of π
    // -----------------------------------------------------------
    for (long long i = 0; i < numSamples; ++i) {
        double x = dist(rng);  // random x in [0,1]
        double y = dist(rng);  // random y in [0,1]
        if (x*x + y*y <= 1.0) {
            insideCircle++;    // point lies inside quarter-circle
        }
    }

    // -----------------------------------------------------------
    // Compute π estimate
    // -----------------------------------------------------------
    // Formula: π ≈ 4 * (points inside circle / total points)
    // -----------------------------------------------------------
    double piEstimate = 4.0 * insideCircle / numSamples;

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Serial Monte Carlo Pi estimate: " << piEstimate << "\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    return 0;
}
