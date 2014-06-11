#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <functional>

typedef std::function<void()> task_t;

class thread_pool
{
public:
    thread_pool(int n_of_threads);
    ~thread_pool();
    void add_task(task_t task);
};

#endif // THREAD_POOL_H
