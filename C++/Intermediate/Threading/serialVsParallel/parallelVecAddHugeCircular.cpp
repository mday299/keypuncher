#include <png.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

// Map distance to a color band
void distanceToColor(float dist, unsigned char& r, unsigned char& g, unsigned char& b) {
    if (dist < 500) { r = 255; g = 0; b = 0; }        // Red center
    else if (dist < 2000) { r = 0; g = 255; b = 0; }  // Green ring
    else if (dist < 4000) { r = 0; g = 0; b = 255; }  // Blue ring
    else if (dist < 6000) { r = 255; g = 255; b = 0; } // Yellow ring
    else { r = 255; g = 255; b = 255; }               // White background
}

// Worker function for a block of rows
void computeRows(int startRow, int endRow,
                 int width, int height,
                 int cx, int cy,
                 std::vector<unsigned char>& image)
{
    for (int j = startRow; j < endRow; ++j) {
        for (int i = 0; i < width; ++i) {
            float dist = std::sqrt((i - cx) * (i - cx) + (j - cy) * (j - cy));
            unsigned char r, g, b;
            distanceToColor(dist, r, g, b);
            size_t idx = (j * width + i) * 3;
            image[idx + 0] = r;
            image[idx + 1] = g;
            image[idx + 2] = b;
        }
    }
}

// Save PNG using libpng
void savePNG(const char* filename, int width, int height, const std::vector<unsigned char>& image) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) { std::cerr << "Failed to open file\n"; return; }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);
    if (!png || !info) { fclose(fp); return; }

    if (setjmp(png_jmpbuf(png))) { fclose(fp); return; }

    png_init_io(png, fp);
    png_set_IHDR(png, info, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png, info);

    std::vector<png_bytep> row_pointers(height);
    for (int j = 0; j < height; ++j) {
        row_pointers[j] = (png_bytep)&image[j * width * 3];
    }

    png_write_image(png, row_pointers.data());
    png_write_end(png, nullptr);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

int main() {
    const int width = 10000;
    const int height = 10000;
    const size_t N = width * height * 3;

    std::vector<unsigned char> image(N);

    int cx = width / 2;
    int cy = height / 2;

    auto start = std::chrono::high_resolution_clock::now();

    unsigned numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    std::vector<std::thread> threads;
    int rowsPerThread = height / numThreads;

    for (unsigned t = 0; t < numThreads; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? height : startRow + rowsPerThread;
        threads.emplace_back(computeRows, startRow, endRow,
                             width, height, cx, cy,
                             std::ref(image));
    }

    for (auto& th : threads) th.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Parallel computation time: " << elapsed.count() << " seconds\n";

    savePNG("circular_parallel.png", width, height, image);
    std::cout << "Image written to circular_parallel.png\n";
}
