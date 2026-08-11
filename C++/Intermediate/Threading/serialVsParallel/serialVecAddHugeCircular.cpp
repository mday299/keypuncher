#include <png.h>              // libpng: used for writing PNG files
#include <vector>             // std::vector for storing pixel data
#include <iostream>           // std::cout, std::cerr
#include <cmath>              // std::sqrt for distance calculation
#include <chrono>             // timing utilities

// ---------------------------------------------------------------
// distanceToColor
// ---------------------------------------------------------------
// Given a distance from the image center, assign an RGB color.
// This creates concentric colored rings based on distance thresholds.
//
// dist: distance from center
// r,g,b: output color channels (0–255)
// ---------------------------------------------------------------
void distanceToColor(float dist, unsigned char& r, unsigned char& g, unsigned char& b) {
    if (dist < 500) { r = 255; g = 0; b = 0; }        // Red center
    else if (dist < 2000) { r = 0; g = 255; b = 0; }  // Green ring
    else if (dist < 4000) { r = 0; g = 0; b = 255; }  // Blue ring
    else if (dist < 6000) { r = 255; g = 255; b = 0; } // Yellow ring
    else { r = 255; g = 255; b = 255; }               // White background
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
// This function:
//   1. Opens a file for binary writing
//   2. Creates libpng write structures
//   3. Sets PNG header info (bit depth, color type, etc.)
//   4. Provides row pointers into the image buffer
//   5. Writes the PNG to disk
// ---------------------------------------------------------------
void savePNG(const char* filename, int width, int height, const std::vector<unsigned char>& image) {
    FILE* fp = fopen(filename, "wb");
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
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png, info);

    // libpng expects an array of row pointers
    std::vector<png_bytep> row_pointers(height);
    for (int j = 0; j < height; ++j) {
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
// Steps:
//   1. Allocate image buffer
//   2. Compute distance for each pixel (serial loop)
//   3. Map distance to color
//   4. Write PNG to disk
//   5. Print timing
//
// This is intentionally serial to demonstrate baseline performance
// before parallelizing with std::thread or CUDA.
// ---------------------------------------------------------------
int main() {
    const int width = 10000;
    const int height = 10000;
    const size_t N = width * height * 3; //total bytes = RGB per pixel

    std::vector<unsigned char> image(N); // flat RGB buffer

    // Image center coordinates
    int cx = width / 2;
    int cy = height / 2;

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // -----------------------------------------------------------
    // Serial computation of all pixels
    // -----------------------------------------------------------
    // For each pixel (i,j):
    //   1. Compute Euclidean distance to center
    //   2. Convert distance to RGB color
    //   3. Store RGB in the image buffer
    //
    // This loop performs 100 million iterations (10k × 10k),
    // each doing a sqrt + color mapping + memory writes.
    // -----------------------------------------------------------
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            // Compute distance from center
            float dist = std::sqrt((i - cx) * (i - cx) + (j - cy) * (j - cy));

            // Map distance to color bands
            unsigned char r, g, b;
            distanceToColor(dist, r, g, b);

            // Compute linear index into flat RGB buffer
            size_t idx = (j * width + i) * 3;

            // Store pixel color
            image[idx + 0] = r;
            image[idx + 1] = g;
            image[idx + 2] = b;
        }
    }

    // End timing    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Serial computation time: " << elapsed.count() << " seconds\n";

    savePNG("circular_serial.png", width, height, image);
    std::cout << "Image written to circular_serial.png\n";
}
