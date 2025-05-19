#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <chrono>

//Warning if you use more than this you run the risk
//of running out of memory. This is not a threading turorial
//it is a Docker tutorial!
#define SMALL_DATA_SIZE 5000

class Sorters {
public:
    static void bubbleSort(std::vector<int>& arr) {
        int size = arr.size();
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - i - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    static void quickSort(std::vector<int>& arr) {
        quickSortHelper(arr, 0, arr.size() - 1);
    }

    static void mergeSort(std::vector<int>& arr) {
        mergeSortHelper(arr, 0, arr.size() - 1);
    }

    static void parallelSort(std::vector<int>& arr) {
        std::sort(std::execution::par, arr.begin(), arr.end());
    }

    static void benchmark(void (*sortFunction)(std::vector<int>&), std::vector<int>& arr, const std::string& name) {
        auto start = std::chrono::high_resolution_clock::now();
        sortFunction(arr);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cout << name << " took " << elapsed.count() << " seconds.\n";
    }

private:
    static void quickSortHelper(std::vector<int>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSortHelper(arr, low, pi - 1);
            quickSortHelper(arr, pi + 1, high);
        }
    }

    static int partition(std::vector<int>& arr, int low, int high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    static void mergeSortHelper(std::vector<int>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortHelper(arr, left, mid);
            mergeSortHelper(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

    static void merge(std::vector<int>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1, n2 = right - mid;
        std::vector<int> L(n1), R(n2);

        for (int i = 0; i < n1; i++) L[i] = arr[left + i];
        for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }
};
