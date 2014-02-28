#pragma once
#include <boost/thread.hpp>
#include <queue>

template<typename T>
class ThreadSafeQueue
{
    std::queue<T> q_;
    boost::mutex mtx;
    boost::condition_variable cond;

public:
    void pop(T& item)
    {
        boost::unique_lock<boost::mutex> lk(mtx);
        cond.wait(lk, [&]() { return !q_.empty(); });
        item = q_.front();
        q_.pop();
    }

    bool try_pop(T& item)
    {
        boost::lock_guard<boost::mutex> lk(mtx);
        if(q_.empty())
            return false;
        item = q_.front();
        q_.pop();
        return true;
    }

    void push(T item)
    {
        boost::lock_guard<boost::mutex> lk(mtx);
        q_.push(item);
        cond.notify_one();
    }

    bool is_empty()
    {
        return q_.empty();
    }
};
