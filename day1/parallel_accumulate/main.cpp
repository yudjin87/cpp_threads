#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

//template<typename It, typename T>
//void accumulate_by_ref(It begin, It end, T& result)
//{
//    result = accumulate(begin, end, T());
//}

template<typename It, typename T>
T parallel_accumulate(It begin, It end, T init)
{
    int N = 4;// = thread::hardware_concurrency();
    vector<thread> threads;
    vector<T> results(N);
    size_t block_size = distance(begin, end)/N;

    It block_start = begin;
    for(int i = 0 ; i < N ; ++i)
    {
        It block_end = block_start;
        advance(block_end, block_size);
        if (i == N-1) block_end = end;
//        threads.emplace_back(&accumulate_by_ref<It, T>,
//                             block_start, block_end,
//                             ref(results[i]));
        threads.emplace_back(
                 [block_start, block_end, &results, i] ()
                    { results[i] =
                      accumulate(block_start, block_end, T() );}
                            );
        block_start = block_end;
    }

    for (thread& th : threads) th.join();

    return accumulate(results.begin(), results.end(), init);
}

int main()
{
    const size_t BIG = 1'000'000;
    cout << "Hello World!" << endl;
    vector<unsigned long> vec1;
    for (int i = 0 ; i < BIG ; ++i)
    {
        vec1.push_back(i);
    }

    vector<unsigned long> vec2;
    for (int i = 0 ; i < BIG ; ++i)
    {
        vec2.push_back(i);
    }

    cout << "N of cores " << thread::hardware_concurrency() << endl;

    auto time_start = chrono::high_resolution_clock::now();
    cout << "Just the Sum = " << accumulate(vec1.begin(), vec1.end(), 0UL) << " ";
    auto time_end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::microseconds>(time_end-time_start).count();
    cout << " um" << endl;


    time_start = chrono::high_resolution_clock::now();
    cout << "Parallel Sum = " << parallel_accumulate(vec2.begin(), vec2.end(), 0UL)  << " ";
    time_end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::microseconds>(time_end-time_start).count();
    cout << " um" << endl;
    return 0;
}



















