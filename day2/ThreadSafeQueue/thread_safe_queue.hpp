#include <boost/thread.hpp>
#include <queue>

template<typename T>
class ThreadSafeQueue
{
    std::queue<T> q;
    boost::mutex mtx;
    boost::condition_variable cond;
public:
    void pop(T& item)
    {
        boost::unique_lock<boost::mutex> lock(mtx);
        while (is_empty())
        {
            cond.wait(lock);
        }
        item = q.front();
        q.pop();
    }

    bool try_pop(T& item)
    {
        boost::lock_guard<boost::mutex> lock(mtx);
        if (is_empty())
        {
            return false;
        }
        item = q.front();
        q.pop();
        return true;
    }

    void push(T item)
    {
        boost::lock_guard<boost::mutex> lock(mtx);
        q.push(item);
        cond.notify_one();
    }

    bool is_empty()
    {
        return q.empty();
    }

};
