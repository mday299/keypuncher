#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

// Worker function: compute a block of rows
void multiplyBlock(const std::vector<double>& A,
                   const std::vector<double>& B,
                   std::vector<double>& C,
                   int N, int startRow, int endRow)
{
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

int main() {
    const int N = 1000; // matrix size (1000 x 1000)
    std::vector<double> A(N * N), B(N * N), C(N * N, 0.0);

    // Initialize matrices with some values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = (i + j) % 100;
            B[i * N + j] = (i - j) % 100;
        }
    }

    unsigned numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4; // fallback

    std::vector<std::thread> threads;
    int rowsPerThread = N / numThreads;

    auto start = std::chrono::high_resolution_clock::now();

    // Launch threads
    for (unsigned t = 0; t < numThreads; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? N : startRow + rowsPerThread;
        threads.emplace_back(multiplyBlock,
                             std::cref(A), std::cref(B), std::ref(C),
                             N, startRow, endRow);
    }

    // Join threads
    for (auto& th : threads) th.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Parallel matrix multiplication (" << N << "x" << N << ") completed with "
              << numThreads << " threads.\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    // Print a sample result to verify correctness
    std::cout << "C[0][0] = " << C[0] << "\n";
    std::cout << "C[N-1][N-1] = " << C[(N-1)*N + (N-1)] << "\n";

    return 0;
}
