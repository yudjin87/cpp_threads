#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <chrono>


int main(int argc, char *argv[])
{
    std::cout << "Hardware threads: "  << std::thread::hardware_concurrency() << std::endl;
    const size_t SIZE = 500000000;
    std::vector<long> v(SIZE);
    for (size_t i = 0 ; i < SIZE; ++i)
    {
        v[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Accumulate:          " << std::accumulate(v.begin(), v.end(), 0);
    auto end = std::chrono::high_resolution_clock::now();
    float seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "   " << seconds << " msec" << std::endl;

    std::vector<long> v2(SIZE);
    for (size_t i = 0 ; i < SIZE; ++i)
    {
        v2[i] = i;
    }

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Parallel Accumulate: " << parallel_accumulate(v2.begin(), v2.end(), 0);
    end = std::chrono::high_resolution_clock::now();
    seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "   " << seconds << " msec" << std::endl;
    return 0;
}
