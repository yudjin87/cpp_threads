#include <iostream>
#include <queue>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>


boost::mutex mtx;
boost::condition_variable cond;
std::queue<int> q;

void producer()
{
    for (int i = 0 ; i < 100 ; ++i)
    {
        std::cout << "Producing " << i << std::endl;
        boost::lock_guard<boost::mutex> lock(mtx);
        boost::this_thread::sleep_for(boost::chrono::milliseconds(300));
        q.push(i);
        cond.notify_one();
    }
    boost::lock_guard<boost::mutex> lock(mtx);
    q.push(-1);
    cond.notify_all();

}

void consumer( int time)
{
    while(true)
    {
        boost::unique_lock<boost::mutex> lock(mtx);
        cond.wait(lock, !boost::bind(&std::queue<int>::empty, &q));
        int msg = q.front();
        if (msg == -1)
        {
            return;
        }
        boost::this_thread::sleep_for(boost::chrono::milliseconds(time));
        q.pop();

        std::cout << "Got " << msg << " " << time << std::endl;
    }
}

int main()
{
    boost::thread cons1(consumer, 90);
    boost::thread cons2(consumer, 110);
    boost::thread prod(producer);
    prod.join();
    cons1.join();
    cons2.join();
}


