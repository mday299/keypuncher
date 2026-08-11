#include <iostream>
#include <chrono>
#include <cuda_runtime.h>

// GPU kernel: each thread computes one element of C
__global__ void matMulKernel(const double* A, const double* B, double* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        double sum = 0.0;
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

int main() {
    const int N = 1000; // matrix size (1000 x 1000)
    size_t bytes = N * N * sizeof(double);

    // Allocate host memory
    double* h_A = new double[N * N];
    double* h_B = new double[N * N];
    double* h_C = new double[N * N];

    // Initialize matrices
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            h_A[i * N + j] = (i + j) % 100;
            h_B[i * N + j] = (i - j) % 100;
        }
    }

    // Allocate device memory
    double *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, bytes);
    cudaMalloc(&d_B, bytes);
    cudaMalloc(&d_C, bytes);

    // Copy data to device
    cudaMemcpy(d_A, h_A, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, bytes, cudaMemcpyHostToDevice);

    auto start = std::chrono::high_resolution_clock::now();

    // Configure grid and block dimensions
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((N + threadsPerBlock.x - 1) / threadsPerBlock.x,
                   (N + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // Launch kernel
    matMulKernel<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, N);
    cudaDeviceSynchronize();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Copy result back
    cudaMemcpy(h_C, d_C, bytes, cudaMemcpyDeviceToHost);

    std::cout << "GPU matrix multiplication (" << N << "x" << N << ") completed.\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    // Sample results
    std::cout << "C[0][0] = " << h_C[0] << "\n";
    std::cout << "C[N-1][N-1] = " << h_C[(N-1)*N + (N-1)] << "\n";

    // Free memory
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
