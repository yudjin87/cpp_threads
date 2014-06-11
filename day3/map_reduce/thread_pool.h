#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <functional>
#include "thread_safe_queue.h"
#include <vector>
#include <thread>
#include <pthread.h>
#include <future>

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

    template<typename T, typename F>
    std::future<T> add_f_task(F fun)
    {
        auto task = std::make_shared<std::packaged_task<T()>>(fun);
        std::future<T> res = task->get_future();
        q.push([task] () { (*task)();} );
        return res;
    }

};

#endif // THREAD_POOL_H
