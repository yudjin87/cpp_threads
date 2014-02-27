#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <stdexcept>

std::mutex mtx;

template <typename LOCK>
class LockGuard
{
    LOCK &mtx_;
public:
    LockGuard(LOCK& mtx) : mtx_(mtx)
    {
        mtx_.lock();
    }
    ~LockGuard()
    {
        mtx_.unlock();
    }
};

void hello(bool fck)
{
    try
    {
        for (int i = 0 ; i < 10 ; ++i)
        {
            //mtx.lock();
            {
                //LockGuard<std::mutex> lock(mtx);
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "Hello from " << std::this_thread::get_id() << std::endl;
                if (fck && i == 5)
                {
                    throw std::bad_alloc();
                }
            }
            //mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (...)
    {
        std::cout << "Exception!!" << std::endl;
    }
}

int main ()
{
    try
    {
        std::thread th1(hello, true);
        std::thread th2(hello, false);
        th1.join();
        th2.join();
    }
    catch (...)
    {
        std::cout << "Exception!!" << std::endl;
    }

}
