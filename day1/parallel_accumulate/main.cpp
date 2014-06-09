#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>

using namespace std;

//template<typename It, typename T>
//void accumulate_by_ref(It begin, It end, T& result)
//{
//    result = accumulate(begin, end, T());
//}

template<typename It, typename T>
T parallel_accumulate(It begin, It end, T init)
{
    int N = thread::hardware_concurrency();
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
    const size_t BIG = 1000000;
    cout << "Hello World!" << endl;
    vector<unsigned long> vec;
    for (int i = 0 ; i < BIG ; ++i)
    {
        vec.push_back(i);
    }
    cout << "N of cores " << thread::hardware_concurrency() << endl;
    cout << "Just the Sum = " << accumulate(vec.begin(), vec.end(), 0UL) << endl;
    cout << "Parallel Sum = " << parallel_accumulate(vec.begin(), vec.end(), 0UL) << endl;

    return 0;
}

