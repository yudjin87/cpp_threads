#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class thread_safe_queue
{
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable cond;

public:
    thread_safe_queue() {}

    void push(T item)
    {
        std::lock_guard<std::mutex> l(mtx);
        q.push(item);
        cond.notify_one();
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> pop_guard(mtx);
        cond.wait(pop_guard, [this] () { return !q.empty(); } );
        item = q.front();
        q.pop();
    }

    bool try_pop(T& item)
    {
        std::unique_lock<std::mutex> pop_guard(mtx);
        if (q.empty())
            return false;
        item = q.front();
        q.pop();
        return true;
    }

};

#endif // THREAD_SAFE_QUEUE_H
