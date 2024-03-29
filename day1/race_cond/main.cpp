#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

atomic<unsigned long> counter(0);

void increment()
{
    for (size_t i = 0 ; i < 20000 ; ++i)
        counter.fetch_add(1, std::memory_order_relaxed); // optimal
        // ++counter // suboptimal on ARMv7 and PowerPC
}

int main()
{
    vector<thread> threads;
    for (size_t i = 0 ; i < 4 ; ++i)
        threads.emplace_back(increment);
    for (auto &th : threads) th.join();
    cout << "Counter should be = 80000, and is = " << counter << endl;
    return 0;
}

