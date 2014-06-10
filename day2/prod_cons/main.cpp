#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

queue<int> q;
mutex q_mtx;
mutex c_mtx;
condition_variable cond;

void producer()
{
    for (int i = 1 ; i <= 100 ; ++i)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
        {
            lock_guard<mutex> pguard(q_mtx);
            q.push(i);
            cond.notify_one();
        }
        {
            lock_guard<mutex> clock(c_mtx);
            cout << "produced " << i << endl;
        }
    }
}

void consumer(int id)
{
    while(true)
    {
        unique_lock<mutex> pop_guard(q_mtx);
        {
//            while (! (!q.empty()) )
//            {
//                cond.wait(pop_guard);
//                // 1. unlocks the lock
//                // 2. waits for notify_**
//                // 3. after getting notifying - get lock locked again
//            }
            cond.wait(pop_guard, [] () { return !q.empty(); } );
            int i = q.front();
            q.pop();
            {
                lock_guard<mutex> clock(c_mtx);
                cout << "cons " << id << " got: " << i << endl;
            }
        }
    }
}

int main()
{
    cout << "Hello Prod - cons!" << endl;
    thread pr_thread(producer);
    thread cons1_thread(consumer, 1);
    thread cons2_thread(consumer, 2);
    thread cons3_thread(consumer, 3);
    pr_thread.join();
    cons1_thread.join();
    cons2_thread.join();
    cons3_thread.join();
    return 0;
}

