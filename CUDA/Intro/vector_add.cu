#include <iostream>
#include <cuda_runtime.h> //cudaMalloc, cudaMemcpy, etc.

//CUDA kernel: add two vectors element-wise
//__global__ marks this as a CUDA kernel (runs on the GPU, callable from the CPU).
__global__ void vectorAdd(const int *a, const int *b, int *c, int n) {
    //blockIdx.x: index of the current block in the grid.
    //blockDim.x: number of threads per block.
    //threadIdx.x: index of the current thread inside its block.
    //Together, i = blockIdx.x * blockDim.x + threadIdx.x computes a global thread index.
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    //The if (i < n) guard ensures threads beyond the vector length don’t access
    // memory out of bounds.
    if (i < n) {
        //Each thread computes one element of the sum: 
        c[i] = a[i] + b[i];
    }
}

int main() {
    const int N = 16;
    size_t size = N * sizeof(int);

    //Host vectors
    //Allocates three arrays on the host (CPU): input vectors h_a, h_b, and output vector h_c.
    //Initializes h_a with values 0..15.
    //Initializes h_b with multiples of 10 (0, 10, 20, …, 150).
    int h_a[N], h_b[N], h_c[N];
    for (int i = 0; i < N; i++) {
        h_a[i] = i;
        h_b[i] = i * 10;
    }

    //Device vectors
    //Declares pointers for arrays on the device (GPU).
    int *d_a, *d_b, *d_c;

    //Allocates GPU memory for each vector using cudaMalloc.
    cudaMalloc((void**)&d_a, size);
    cudaMalloc((void**)&d_b, size);
    cudaMalloc((void**)&d_c, size);

    //Copies initialized data from host arrays (h_a, h_b) into device memory (d_a, d_b).
    //Direction flag cudaMemcpyHostToDevice specifies the transfer direction.
    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    //Launch kernel with enough threads
    int threadsPerBlock = 8; //each block has 8 threads
    //Ensures enough blocks to cover all 16 elements.
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
    //Launch kernel call
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, N);

    //waits until all GPU work finishes before continuing.
    cudaDeviceSynchronize(); 
    // handle misaligned CUDA runtimes gracefully
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA kernel error: " << cudaGetErrorString(err) << std::endl;
    }

    //Copies the results from GPU memory (d_c) back into the host array (h_c).
    cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);

    // Print results
    for (int i = 0; i < N; i++) {
        std::cout << h_a[i] << " + " << h_b[i] << " = " << h_c[i] << std::endl;
    }

    //Frees GPU memory to avoid leaks.
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}