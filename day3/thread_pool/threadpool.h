#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <vector>
#include "thread_safe_queue.hpp"

typedef std::function<void()> Task;

class ThreadPool
{
private:
    ThreadSafeQueue<Task> queue;
    std::vector<std::thread> workers;
    int n_of_threads;

public:
    ThreadPool(int n_of_threads)
        : n_of_threads(n_of_threads)
    {
        for (int i = 0 ; i < n_of_threads ; ++i)
        {
            workers.emplace_back( [this]()
            {
                while(true)
                {
                    Task task;
                    queue.pop(task);
                    if (task)
                        task();
                    else
                        return;
                }
            });
        }
    }

    ~ThreadPool()
    {
        for (int i = 0 ; i < n_of_threads ; ++i)
        {
            queue.push(nullptr);
        }
        for (auto& thd : workers)
        {
            thd.join();
        }
    }

    void add_task(Task f)
    {
        queue.push(f);
    }


    template< class T, class F>
    std::future<T> add_future_task(F f)
    {
        auto task = std::make_shared<std::packaged_task<T()>>(f);
        std::future<T> res = task->get_future();
        queue.push( [task]() { (*task)();} );
        return res;
    }
};
