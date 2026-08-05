#include <opencv2/opencv.hpp>    // Core OpenCV functionality
#include <iostream>              // Standard C++ I/O for error messages and logging

int main() {
    // Step 1: Load image from file
    // cv::imread reads an image into a cv::Mat object.
    // "openCV-sample.png" must exist in the working directory.
    cv::Mat input = cv::imread("openCV-sample.png");
    if (input.empty()) {
        std::cerr << "Error: Could not load image!" << std::endl;
        return -1;
    }

    // Step 2: Convert to grayscale
    // cv::cvtColor transforms the color space of the image.
    // Here, we convert from BGR (default OpenCV format) to grayscale.
    cv::Mat gray;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

    // Step 3: Apply Gaussian blur
    // cv::GaussianBlur smooths the image, reducing noise and detail.
    // Kernel size (7x7) and sigma (1.5) control the strength of the blur.
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(7, 7), 1.5);

    // Step 4: Edge detection (Canny)
    // cv::Canny detects edges by looking for intensity gradients.
    // Thresholds (50, 150) control sensitivity:
    //   - Lower threshold: potential edges
    //   - Upper threshold: strong edges
    cv::Mat edges;
    cv::Canny(blurred, edges, 50, 150);

    // Step 5: Display results in separate windows
    // cv::imshow creates a window and shows the image.
    cv::imshow("Original", input);
    cv::imshow("Grayscale", gray);
    cv::imshow("Blurred", blurred);
    cv::imshow("Edges", edges);

    // Main loop:
    while (true) {
        int key = cv::waitKey(30);

        // If all windows are closed, exit
        if (cv::getWindowProperty("Original", cv::WND_PROP_VISIBLE) < 1 &&
            cv::getWindowProperty("Grayscale", cv::WND_PROP_VISIBLE) < 1 &&
            cv::getWindowProperty("Blurred", cv::WND_PROP_VISIBLE) < 1 &&
            cv::getWindowProperty("Edges", cv::WND_PROP_VISIBLE) < 1) {
            break;
        }
    }

    return 0;
}
