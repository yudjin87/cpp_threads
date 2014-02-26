#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <chrono>

template <typename It, typename T>
void partial_accumulate(It first, It last, T& result)
{
    result = std::accumulate(first, last, result);
}

template <typename It, typename T>
T parallel_accumulate(It first, It last, T init)
{
    size_t len =  std::distance(first, last);
    if(!len) return init;
    unsigned int hw_thd = std::thread::hardware_concurrency();
    size_t block_size = len/hw_thd;

    std::vector<T> results(hw_thd);
    std::vector<std::thread> threads;
    // creating threads

    It block_start = first;
    for (unsigned int i = 0 ; i < hw_thd-1 ; ++i)
    {
        It block_end = block_start;
        std::advance(block_end, block_size);
        threads.emplace_back( partial_accumulate<It, T>,
                              block_start, block_end,
                              std::ref(results[i]));
        block_start = block_end;
    }
    threads.emplace_back( partial_accumulate<It, T>,
                          block_start, last,
                          std::ref(results[hw_thd-1]));

    // joining threads
    for (auto &thread : threads)
        thread.join();
    return std::accumulate(results.begin(), results.end(), init);
}

int main(int argc, char *argv[])
{
    std::cout << "Hardware threads: "  << std::thread::hardware_concurrency() << std::endl;
    const size_t SIZE = 5000000;
    std::vector<long> v(SIZE);
    for (size_t i = 0 ; i < SIZE; ++i)
    {
        v[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Accumulate:          " << std::accumulate(v.begin(), v.end(), 0);
    auto end = std::chrono::high_resolution_clock::now();
    float seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    std::cout << "   " << seconds << " ns" << std::endl;

    std::vector<long> v2(SIZE);
    for (size_t i = 0 ; i < SIZE; ++i)
    {
        v2[i] = i;
    }

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Parallel Accumulate: " << parallel_accumulate(v2.begin(), v2.end(), 0);
    end = std::chrono::high_resolution_clock::now();
    seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    std::cout << "   " << seconds << " ns" << std::endl;
    return 0;
}
