#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

thread_local int counter = 0;

void worker()
{
    for (int i = 0 ; i < 25 ; ++i)
    {
        cout << "Counter in thread " << this_thread::get_id();
        cout << " " << counter << endl;
        this_thread::sleep_for(chrono::microseconds(10));
        ++counter;
    }
}

int main()
{
    cout << "counter in main = " << counter << endl;
    thread th1(&worker);
    thread th2(&worker);
    th1.join();
    th2.join();
    cout << "counter in main after threads = " << counter << endl;
}
