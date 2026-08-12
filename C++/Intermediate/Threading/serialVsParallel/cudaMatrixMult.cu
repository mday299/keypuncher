#include <iostream>       // For console output (std::cout)
#include <chrono>         // For timing execution
#include <cuda_runtime.h> // CUDA runtime API (cudaMalloc, cudaMemcpy, etc.)

// ---------------------------------------------------------------
// matMulKernel (GPU kernel)
// ---------------------------------------------------------------
// Each GPU thread computes one element of the output matrix C.
// Thread indices (row, col) map directly to matrix coordinates.
//
// Parameters:
//   A, B: input matrices (flattened into 1D arrays, row-major order)
//   C: output matrix (flattened into 1D array)
//   N: matrix dimension (N x N)
// ---------------------------------------------------------------
__global__ void matMulKernel(const double* A, const double* B, double* C, int N) {
    // Compute row and column indices from block + thread indices
    int row = blockIdx.y * blockDim.y + threadIdx.y; // row index
    int col = blockIdx.x * blockDim.x + threadIdx.x; // column index

    // Bounds check: only process valid elements
    if (row < N && col < N) {
        double sum = 0.0;
        // Dot product of row 'row' of A and column 'col' of B
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        // Store result in C[row][col]
        C[row * N + col] = sum;
    }
}

int main() {
    const int N = 1000; // matrix size (1000 x 1000)
    size_t bytes = N * N * sizeof(double); // total memory required per matrix

    // -----------------------------------------------------------
    // Allocate host (CPU) memory for matrices A, B, and C
    // -----------------------------------------------------------
    double* h_A = new double[N * N];
    double* h_B = new double[N * N];
    double* h_C = new double[N * N];

    // -----------------------------------------------------------
    // Initialize matrices A and B with deterministic values
    //   A[i][j] = (i + j) % 100
    //   B[i][j] = (i - j) % 100
    // Stored in row-major order
    // -----------------------------------------------------------    
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

    // -----------------------------------------------------------
    // Copy input matrices A and B from host to device
    // -----------------------------------------------------------
    cudaMemcpy(d_A, h_A, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, bytes, cudaMemcpyHostToDevice);

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Configure grid and block dimensions
    // -----------------------------------------------------------
    // Each block has 16x16 = 256 threads.
    // Grid dimensions are chosen to cover the entire N x N matrix.
    //   numBlocks.x = ceil(N / 16)
    //   numBlocks.y = ceil(N / 16)
    // -----------------------------------------------------------
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((N + threadsPerBlock.x - 1) / threadsPerBlock.x,
                   (N + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // Calculate total threads launched
    int totalThreadsX = numBlocks.x * threadsPerBlock.x;
    int totalThreadsY = numBlocks.y * threadsPerBlock.y;
    long long totalThreads = (long long)totalThreadsX * (long long)totalThreadsY;

    std::cout << "Threads per block: (" 
            << threadsPerBlock.x << ", " << threadsPerBlock.y << ")\n";
    std::cout << "Blocks per grid: (" 
            << numBlocks.x << ", " << numBlocks.y << ")\n";
    std::cout << "Total threads in X dimension: " << totalThreadsX << "\n";
    std::cout << "Total threads in Y dimension: " << totalThreadsY << "\n";
    std::cout << "Total threads launched: " << totalThreads << "\n";

    // -----------------------------------------------------------
    // Launch kernel: each thread computes one element of C
    // -----------------------------------------------------------
    matMulKernel<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, N);
    cudaDeviceSynchronize(); // wait for GPU to finish

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    // -----------------------------------------------------------
    // Copy result matrix C back from device to host
    // -----------------------------------------------------------
    cudaMemcpy(h_C, d_C, bytes, cudaMemcpyDeviceToHost);

    // Print timing results
    std::cout << "GPU matrix multiplication (" << N << "x" << N << ") completed.\n";
    std::cout << "Computation time: " << elapsed.count() << " seconds\n";

    // -----------------------------------------------------------
    // Print sample results to verify correctness
    //   C[0][0] → first element
    //   C[N-1][N-1] → last element
    // -----------------------------------------------------------
    std::cout << "C[0][0] = " << h_C[0] << "\n";
    std::cout << "C[N-1][N-1] = " << h_C[(N-1)*N + (N-1)] << "\n";

    // Free host and device memory
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
