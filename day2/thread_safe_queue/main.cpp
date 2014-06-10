#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "thread_safe_queue.h"

using namespace std;

thread_safe_queue<int> q;

mutex c_mtx;

void print(const string& msg, int id, int value )
{

    lock_guard<mutex> clock(c_mtx);
    cout << id << " " << msg << " " << value << endl;
}

void producer()
{
    for (int i = 1 ; i <= 10 ; ++i)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
        q.push(i);
        print("producer", 0, i);
    }
    q.push(-1);
}

void consumer(int id)
{
    while(true)
    {
        int i;
        q.pop(i);
        if (i == -1)
        {
            q.push(-1);
            return;
        }
        print("consumer got", id, i);
    }
}

int main()
{
    cout << "Hello Prod - cons!" << endl;
    thread cons1_thread(consumer, 1);
    thread cons2_thread(consumer, 2);
    thread cons3_thread(consumer, 3);
    producer();
    cons1_thread.join();
    cons2_thread.join();
    cons3_thread.join();
    return 0;
}

