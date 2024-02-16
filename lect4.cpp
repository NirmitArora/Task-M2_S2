#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

void parallelAddition(int* v1, int* v2, int* v3, int size, int num_threads) {
    vector<thread> threads;

    // Function to be executed by each thread
    auto addPartition = [&](int start, int end) {
        for (int i = start; i < end; i++) {
            v3[i] = v1[i] + v2[i];
        }
    };

    // Divide the workload among threads
    int chunk_size = size / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        threads.emplace_back(addPartition, start, end);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {
    const unsigned long size = 100000000;
    const int num_threads = 4;

    int* v1 = new int[size];
    int* v2 = new int[size];
    int* v3 = new int[size];

    srand(time(0));

    // Fill v1 and v2 with random numbers
    randomVector(v1, size);
    randomVector(v2, size);

    auto start = high_resolution_clock::now();

    // Perform parallel addition
    parallelAddition(v1, v2, v3, size, num_threads);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    return 0;
}
