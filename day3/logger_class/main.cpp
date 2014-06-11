#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include "thread_safe_queue.h"

typedef std::function<void()> task_t;

using namespace std;

class ActiveObject
{
    thread_safe_queue<task_t> q;
public:
    ActiveObject()
    {

    }

    ~ActiveObject()
    {

    }

    void send(task_t task)
    {

    }
};

class Logger
{
    ofstream fout;
    mutex mtx;
public:
    Logger(const string& fname)
    {
        fout.open(fname);
    }

    ~Logger()
    {
        fout.close();
    }

    void log(const string& message)
    {
        lock_guard<mutex> l(mtx);
        this_thread::sleep_for(chrono::milliseconds(10));
        fout << message << endl;
    }
};

void log_id(Logger& logger, int id)
{
    for (int i = 0 ; i < 100 ; ++i)
    {
        logger.log("Log#" + to_string(id) + " : message number " + to_string(i));
    }
}

int main()
{
    cout << "Hello Logger!" << endl;
    Logger logger("log.txt");
    vector<thread> threads;
    for (int i = 0 ; i < 10 ; ++i)
        threads.emplace_back(log_id, ref(logger), i);

    for (thread& th : threads) th.join();
    return 0;
}

