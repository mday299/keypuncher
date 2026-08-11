#include <vector>
#include <iostream>

void vectorAddSerial(const std::vector<float>& A,
                     const std::vector<float>& B,
                     std::vector<float>& C)
{
    size_t n = A.size();
    for (size_t i = 0; i < n; ++i) {
        C[i] = A[i] + B[i];
    }
}

int main() {
    std::vector<float> A = {1.0f, 2.0f, 3.0f};
    std::vector<float> B = {4.0f, 5.0f, 6.0f};
    std::vector<float> C(3);

    vectorAddSerial(A, B, C);

    for (float x : C) {
        std::cout << x << " ";
    }

    std::cout << std::endl;
}
