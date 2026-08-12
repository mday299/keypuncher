#include <iostream>   // For console output (std::cout)
#include <vector>     // For dynamic arrays (std::vector)
#include <chrono>     // For timing execution
#include <thread>     // For multithreading (std::thread)

// ---------------------------------------------------------------
// multiplyBlock (worker function)
// ---------------------------------------------------------------
// Each thread runs this function to compute a block of rows of C.
// Splitting rows across threads avoids overlap and race conditions.
//
// Parameters:
//   A, B: input matrices (flattened into 1D vectors, row-major order)
//   C: output matrix (flattened into 1D vector)
//   N: matrix dimension (N x N)
//   startRow, endRow: range of rows assigned to this thread
// ---------------------------------------------------------------
void multiplyBlock(const std::vector<double>& A,
                   const std::vector<double>& B,
                   std::vector<double>& C,
                   int N, int startRow, int endRow)
{
    for (int i = startRow; i < endRow; ++i) {  // iterate over assigned rows
        for (int j = 0; j < N; ++j) {          // iterate over all columns
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {      // dot product of row i and column j
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;                // store result in C
        }
    }
}

int main() {
    const int N = 1000; // matrix size (1000 x 1000)

    // -----------------------------------------------------------
    // Allocate matrices A, B, and C as 1D vectors.
    // Each has N*N elements, stored in row-major order.
    //   A and B: input matrices
    //   C: output matrix (initialized to 0.0)
    // -----------------------------------------------------------    
    std::vector<double> A(N * N), B(N * N), C(N * N, 0.0);

    // -----------------------------------------------------------
    // Initialize matrices A and B with simple values.
    // This ensures deterministic results for testing.
    //   A[i][j] = (i + j) % 100
    //   B[i][j] = (i - j) % 100
    // Access pattern: A[i * N + j] maps 2D indices (i,j) to 1D.
    // -----------------------------------------------------------
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = (i + j) % 100;
            B[i * N + j] = (i - j) % 100;
        }
    }

    // -----------------------------------------------------------
    // Determine number of threads to use.
    // std::thread::hardware_concurrency() queries available cores.
    // Fallback to 4 threads if query fails.
    // -----------------------------------------------------------
    unsigned numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4; // fallback

    std::vector<std::thread> threads;   // container for thread objects
    int rowsPerThread = N / numThreads; // divide rows evenly among threads

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // -----------------------------------------------------------
    // Launch threads
    // -----------------------------------------------------------
    // Each thread computes a contiguous block of rows.
    // The last thread handles any remainder rows.
    // -----------------------------------------------------------    
    for (unsigned t = 0; t < numThreads; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? N : startRow + rowsPerThread;
        threads.emplace_back(multiplyBlock,
                             std::cref(A), std::cref(B), std::ref(C),
                             N, startRow, endRow);
    }

    // -----------------------------------------------------------
    // Join threads
    // -----------------------------------------------------------
    // Wait for all threads to finish before proceeding.
    // Ensures matrix C is fully computed before timing ends.
    // -----------------------------------------------------------
    for (auto& th : threads) th.join();

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // -----------------------------------------------------------
    // Print timing results.
    // Shows how long the parallel computation took.
    // -----------------------------------------------------------
    std::cout << "Parallel matrix multiplication (" << N << "x" << N << ") completed with "
              << numThreads << " threads.\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    // -----------------------------------------------------------
    // Print sample results to verify correctness.
    //   C[0][0] → first element
    //   C[N-1][N-1] → last element
    // This avoids printing the entire matrix (too large).
    // -----------------------------------------------------------
    std::cout << "C[0][0] = " << C[0] << "\n";
    std::cout << "C[N-1][N-1] = " << C[(N-1)*N + (N-1)] << "\n";

    return 0;
}
