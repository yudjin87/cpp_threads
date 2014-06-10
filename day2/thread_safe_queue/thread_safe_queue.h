#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

template<typename T>
class thread_safe_queue
{
public:
    thread_safe_queue()
    {

    }

    void push(T item)
    {

    }

    void pop(T& item)
    {

    }

    bool try_pop(T& item)
    {
        return false; // if empty
    }

};

#endif // THREAD_SAFE_QUEUE_H
