#include <iostream>       // For console output (std::cout)
#include <chrono>         // For timing execution
#include <curand_kernel.h>// CUDA random number generation library (CURAND)

// ---------------------------------------------------------------
// monteCarloPiKernel (GPU kernel)
// ---------------------------------------------------------------
// Each GPU thread runs its own random number generator (RNG)
// and performs Monte Carlo sampling to estimate Pi.
//
// Parameters:
//   samplesPerThread: number of random points each thread generates
//   counts: device array to store per-thread counts of points inside circle
//   seed: base seed for RNG initialization
// ---------------------------------------------------------------
__global__ void monteCarloPiKernel(long long samplesPerThread, long long* counts, unsigned long long seed) {
    // Compute global thread ID (unique index across grid)
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    // Initialize CURAND state for this thread
    // curand_init(seed, sequence_number, offset, state)
    //   seed: base seed
    //   sequence_number: ensures different streams per thread
    //   offset: starting point in sequence
    curandState state;
    curand_init(seed, tid, 0, &state);

    long long localCount = 0; // local counter for points inside circle

    // Generate 'samplesPerThread' random points
    for (long long i = 0; i < samplesPerThread; ++i) {
        double x = curand_uniform_double(&state); // random x in [0,1]
        double y = curand_uniform_double(&state); // random y in [0,1]
        if (x*x + y*y <= 1.0) {
            localCount++; // point lies inside quarter-circle
        }
    }

    // Write result back to global memory
    counts[tid] = localCount;
}

int main() {
    const long long totalSamples = 100000000LL; // 100 million samples

    // Grid/block configration
    const int threadsPerBlock = 256;
    const int numBlocks = 256;                  // 256 * 256 = 65,536 threads
    const int totalThreads = threadsPerBlock * numBlocks;

    // Print total number of threads
    std::cout << "Threads per block: " << threadsPerBlock << "\n";
    std::cout << "Blocks per grid: " << numBlocks << "\n";
    std::cout << "Total threads launched: " << totalThreads << "\n";

    // Each thread handles an equal share of samples
    long long samplesPerThread = totalSamples / totalThreads;

    // Allocate device memory for per-thread counts
    long long* d_counts;
    cudaMalloc(&d_counts, totalThreads * sizeof(long long));

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Launch kernel
    // Each of the 65,536 threads runs its own RNG and counts hits
    monteCarloPiKernel<<<numBlocks, threadsPerBlock>>>(samplesPerThread, d_counts, 1234ULL);
    cudaDeviceSynchronize();  // wait for GPU to finish

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Copy results back to host
    long long* h_counts = new long long[totalThreads];
    cudaMemcpy(h_counts, d_counts, totalThreads * sizeof(long long), cudaMemcpyDeviceToHost);
    cudaFree(d_counts);

    // Aggregate results
    long long insideCircle = 0;
    for (int i = 0; i < totalThreads; ++i) {
        insideCircle += h_counts[i];
    }
    delete[] h_counts;

    double piEstimate = 4.0 * insideCircle / (samplesPerThread * totalThreads);

    std::cout << "GPU Monte Carlo Pi estimate: " << piEstimate << "\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    return 0;
}
