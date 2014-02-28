#include <iostream>
#include <thread>
#include <chrono>
#include <future>

#include "threadpool.h"

void newTask(int i)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Task " << i << " is done" <<std::endl;
}

int Question()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return 42;
}

int main()
{
    ThreadPool tp(4);
    // add_task( std::function<void(void)> )
    // zad nr 1
    for (int i = 0 ; i < 20 ; ++i)
        tp.add_task( [i] () { newTask(i);} );

    std::future<int> res = tp.add_future_task<int>(Question);
    std::cout << "Future resut: " << res.get() << std::endl;
}
