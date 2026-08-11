#include <iostream>
#include <vector>
#include <chrono>

int main() {
    const int N = 1000; // matrix size (1,000 x 1,000)
    std::vector<double> A(N * N), B(N * N), C(N * N, 0.0);

    // Initialize matrices with some values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i * N + j] = (i + j) % 100;
            B[i * N + j] = (i - j) % 100;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Serial matrix multiplication
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Serial matrix multiplication (" << N << "x" << N << ") completed.\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    // Print a sample result to verify correctness
    std::cout << "C[0][0] = " << C[0] << "\n";
    std::cout << "C[N-1][N-1] = " << C[(N-1)*N + (N-1)] << "\n";

    return 0;
}
