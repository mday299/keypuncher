#include "sorters.cpp"

int main() {
    std::vector<int> smallData(SMALL_DATA_SIZE);
    
    for (int& num : smallData) num = rand() % SMALL_DATA_SIZE;
    
    std::cout << "Benchmarking Small Dataset:\n";
    Sorters::benchmark(Sorters::bubbleSort, smallData, "Single-Threaded Bubble Sort");
    Sorters::benchmark(Sorters::quickSort, smallData, "Single-Threaded Quick Sort");
    Sorters::benchmark(Sorters::mergeSort, smallData, "Single-Threaded Merge Sort");
    Sorters::benchmark(Sorters::parallelSort, smallData, "Parallel Sort");

    return 0;
}
