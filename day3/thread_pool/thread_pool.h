#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <functional>
#include "thread_safe_queue.h"
#include <vector>
#include <thread>
#include <pthread.h>

typedef std::function<void()> task_t;

class thread_pool
{
    std::vector<std::thread> workers;
    thread_safe_queue<task_t> q;

public:
    thread_pool(int n_of_threads)
    {
        for (int i = 0 ; i < n_of_threads ; ++i)
            //workers.emplace_back(&thread_pool::worker, this);
            workers.emplace_back([this] () {
                for(;;)
                {
                    task_t task;
                    q.pop(task);
                    task();
                }
            });
    }

    ~thread_pool()
    {
        for (std::thread& th : workers)
        {
            q.push( [] () { pthread_exit(0);} );
        }
        for (std::thread& th : workers)
        {
            th.join();
        }
    }

    void add_task(task_t task)
    {
        q.push(task);
    }
};

#endif // THREAD_POOL_H
