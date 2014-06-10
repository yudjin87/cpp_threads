#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <math.h>

using namespace std;

unsigned long counter = 0;
mutex mtx_counter;
double scratch;

void not_safe_to_call()
{
    ++counter;
    if(counter == 19000)
        throw std::exception();
}

class lock_guard
{

};

void increment()
{
    for (size_t i = 0 ; i < 20000 ; ++i)
    {
        lock_guard guard(mtx_counter);
        ++counter;
        if (counter == 19000) continue;
        // some other operation
    }
}

int main()
{
    vector<thread> threads;
    auto time_start = chrono::high_resolution_clock::now();
    for (size_t i = 0 ; i < 4 ; ++i)
        threads.emplace_back(increment);
    for (auto &th : threads) th.join();
    auto time_end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::microseconds>(time_end-time_start).count();
    cout << " um" << endl;
    cout << "Counter should be = 80000, and is = " << counter << endl;
    return 0;
}

