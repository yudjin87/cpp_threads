#include <boost/thread.hpp>
#include <queue>

template<typename T>
class ThreadSafeQueue
{
public:
    void pop(T& item);
    bool try_pop(T& item);
    void push(T item);
    bool is_empty();
};
