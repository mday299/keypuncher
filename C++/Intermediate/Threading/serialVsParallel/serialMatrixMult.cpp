#include <iostream>   // For console output (std::cout)
#include <vector>     // For dynamic arrays (std::vector)
#include <chrono>     // For timing execution

int main() {
    const int N = 1000; // matrix size (1,000 x 1,000)

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

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // -----------------------------------------------------------
    // Serial matrix multiplication (triple nested loop).
    // Formula:
    //   C[i][j] = sum over k of (A[i][k] * B[k][j])
    //
    // Loops:
    //   Outer loop (i): iterate over rows of A
    //   Middle loop (j): iterate over columns of B
    //   Inner loop (k): compute dot product of row i and column j
    //
    // Complexity: O(N^3) operations
    // For N=1000 → 1 billion multiplications + additions
    // -----------------------------------------------------------
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // -----------------------------------------------------------
    // Print timing results.
    // -----------------------------------------------------------
    std::cout << "Serial matrix multiplication (" << N << "x" << N << ") completed.\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    // -----------------------------------------------------------
    // Print sample results to verify correctness.
    //   C[0][0] → first element
    //   C[N-1][N-1] → last element
    // This avoids printing the entire matrix (too large).
    // -----------------------------------------------------------
    std::cout << "C[0][0] = " << C[0] << "\n";
    std::cout << "C[N-1][N-1] = " << C[(N-1)*N + (N-1)] << "\n";
    std::cout << "C[0][0] = " << C[0] << "\n";
    std::cout << "C[N-1][N-1] = " << C[(N-1)*N + (N-1)] << "\n";

    return 0;
}
