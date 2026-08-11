#include <iostream>
#include <chrono>
#include <curand_kernel.h>

// GPU kernel: each thread runs its own RNG and counts hits
__global__ void monteCarloPiKernel(long long samplesPerThread, long long* counts, unsigned long long seed) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    curandState state;
    curand_init(seed, tid, 0, &state);

    long long localCount = 0;
    for (long long i = 0; i < samplesPerThread; ++i) {
        double x = curand_uniform_double(&state);
        double y = curand_uniform_double(&state);
        if (x*x + y*y <= 1.0) {
            localCount++;
        }
    }
    counts[tid] = localCount;
}

int main() {
    const long long totalSamples = 100000000LL; // 100 million samples
    const int threadsPerBlock = 256;
    const int numBlocks = 256;                  // 256 * 256 = 65,536 threads
    const int totalThreads = threadsPerBlock * numBlocks;

    long long samplesPerThread = totalSamples / totalThreads;

    // Allocate device memory
    long long* d_counts;
    cudaMalloc(&d_counts, totalThreads * sizeof(long long));

    auto start = std::chrono::high_resolution_clock::now();

    // Launch kernel
    monteCarloPiKernel<<<numBlocks, threadsPerBlock>>>(samplesPerThread, d_counts, 1234ULL);
    cudaDeviceSynchronize();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Copy results back
    long long* h_counts = new long long[totalThreads];
    cudaMemcpy(h_counts, d_counts, totalThreads * sizeof(long long), cudaMemcpyDeviceToHost);
    cudaFree(d_counts);

    // Aggregate
    long long insideCircle = 0;
    for (int i = 0; i < totalThreads; ++i) {
        insideCircle += h_counts[i];
    }
    delete[] h_counts;

    double piEstimate = 4.0 * insideCircle / (samplesPerThread * totalThreads);

    std::cout << "GPU Monte Carlo π estimate: " << piEstimate << "\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    return 0;
}
