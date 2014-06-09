#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void hello(int id, const string& msg)
{
    this_thread::sleep_for(chrono::milliseconds(100*id));
    cout << "Hello from thread id: " << id << endl;
    cout << "Message: " << msg << endl;
}

int main()
{
    vector<thread> threads;
    //threads.push_back(thread(&hello, 1, "Hello 1"));
    for (int i = 1 ; i <= 20 ; ++i)
        threads.emplace_back(&hello, i, string("Hello ") + to_string(i));

    for (thread& th : threads) th.join();

    return 0;
}

