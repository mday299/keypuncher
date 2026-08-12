#include <png.h>              // libpng: used for writing PNG files
#include <vector>             // std::vector for storing pixel data
#include <iostream>           // std::cout, std::cerr
#include <cmath>              // sqrtf for distance calculation
#include <chrono>             // timing utilities

// ---------------------------------------------------------------
// distanceToColor (device function)
// ---------------------------------------------------------------
// This function runs on the GPU.
// Given a distance from the image center, assign an RGB color.
// Creates concentric colored rings based on distance thresholds.
//
// dist: distance from center
// r,g,b: output color channels (0–255)
// ---------------------------------------------------------------
__device__ void distanceToColor(float dist, unsigned char& r, unsigned char& g, unsigned char& b) {
    if (dist < 500) { r = 255; g = 0; b = 0; }        // Red center
    else if (dist < 2000) { r = 0; g = 255; b = 0; }  // Green ring
    else if (dist < 4000) { r = 0; g = 0; b = 255; }  // Blue ring
    else if (dist < 6000) { r = 255; g = 255; b = 0; } // Yellow ring
    else { r = 255; g = 255; b = 255; }               // White background
}

// GPU kernel
// ---------------------------------------------------------------
// computeImage (GPU kernel)
// ---------------------------------------------------------------
// Each GPU thread computes the color of one pixel.
// Thread indices (i,j) map to pixel coordinates.
//
// width, height: image dimensions
// cx, cy: center coordinates
// image: pointer to device memory storing RGB values
// ---------------------------------------------------------------
__global__ void computeImage(int width, int height, int cx, int cy, unsigned char* image) {
    // Compute pixel coordinates from block + thread indices
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    // Bounds check: only process valid pixels
    if (i < width && j < height) {
        // Compute Euclidean distance from (i,j) to center (cx,cy)
        float dist = sqrtf((i - cx) * (i - cx) + (j - cy) * (j - cy));

        // Map distance to RGB color
        unsigned char r, g, b;
        distanceToColor(dist, r, g, b);

        // Compute linear index into flat RGB buffer
        size_t idx = (j * width + i) * 3;

        // Store pixel color in device memory
        image[idx + 0] = r;
        image[idx + 1] = g;
        image[idx + 2] = b;
    }
}

// ---------------------------------------------------------------
// savePNG
// ---------------------------------------------------------------
// Writes an RGB image buffer to a PNG file using libpng.
//
// filename: output PNG file name
// width, height: image dimensions
// image: flat vector of RGB bytes (size = width * height * 3)
//
// Steps:
//   1. Open file for binary writing
//   2. Create libpng write structures
//   3. Set PNG header info (bit depth, color type, etc.)
//   4. Provide row pointers into the image buffer
//   5. Write PNG to disk
// ---------------------------------------------------------------
void savePNG(const char* filename, int width, int height, const std::vector<unsigned char>& image) {
    FILE* fp = fopen(filename, "wb");  // open file for binary write
    if (!fp) { std::cerr << "Failed to open file\n"; return; }

    // Create PNG write struct + info struct
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);
    if (!png || !info) { fclose(fp); return; }

    // libpng uses setjmp for error handling   
    if (setjmp(png_jmpbuf(png))) { fclose(fp); return; }

    // Initialize PNG IO
    png_init_io(png, fp);

    // Set PNG header: 8-bit RGB, no interlace
    png_set_IHDR(png, info, width, height,
                 8, // bit depth
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png, info);

    // libpng expects an array of row pointers
    std::vector<png_bytep> row_pointers(height);
    for (int j = 0; j < height; ++j) {
        // Each row starts at image[j * width * 3]
        row_pointers[j] = (png_bytep)&image[j * width * 3];
    }

    // Write pixel data
    png_write_image(png, row_pointers.data());
    png_write_end(png, nullptr);

    // Cleanup    
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

// ---------------------------------------------------------------
// main
// ---------------------------------------------------------------
// Generates a 10,000 × 10,000 RGB image containing concentric rings.
// Each pixel's color is determined by its distance from the center.
//
// Parallelization strategy:
//   - Allocate device memory for image
//   - Configure grid/block dimensions
//   - Launch GPU kernel (computeImage)
//   - Synchronize and measure runtime
//   - Copy result back to host
//   - Save final image to PNG
// ---------------------------------------------------------------
int main() {
    const int width = 10000;
    const int height = 10000;
    const size_t N = width * height * 3; // total bytes (RGB per pixel)

    std::vector<unsigned char> image(N); // host RGB buffer

    // Allocate device memory
    unsigned char* d_image;
    cudaMalloc(&d_image, N * sizeof(unsigned char));

    // Image center coordinates
    int cx = width / 2;
    int cy = height / 2;

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Configure grid and block dimensions
    dim3 threadsPerBlock(16, 16); // 16×16 = 256 threads per block
    dim3 numBlocks((width + threadsPerBlock.x - 1) / threadsPerBlock.x, //blocks to cover width
                   (height + threadsPerBlock.y - 1) / threadsPerBlock.y //blocks to cover height
                  );

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

    // Launch kernel: each thread computes one pixel
    computeImage<<<numBlocks, threadsPerBlock>>>(width, height, cx, cy, d_image);
    cudaDeviceSynchronize(); //wait for GPU to finish

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "GPU parallel computation time: " << elapsed.count() << " seconds\n";

    // Copy result back to host
    cudaMemcpy(image.data(), d_image, N * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_image);

    savePNG("circular_gpu.png", width, height, image);
    std::cout << "Image written to circular_gpu.png\n";
}
