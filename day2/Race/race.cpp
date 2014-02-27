#include <iostream>
#include <boost/thread.hpp>
#include <chrono>

boost::mutex c_mtx;

using namespace std;

int counter = 0;

void worker()
{
    c_mtx.lock();
    for (int i = 0 ; i < 10000000 ; ++i)
    {
        counter++;
    }
    c_mtx.unlock();
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    boost::thread th1(worker);
    boost::thread th2(worker);
    th1.join();
    th2.join();
    auto end = std::chrono::high_resolution_clock::now();
    float seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "   " << seconds << " msec" << std::endl;

    cout << "Counter = " << counter << endl;
}

