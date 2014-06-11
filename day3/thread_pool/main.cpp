#include <iostream>
#include "thread_pool.h"

using namespace std;

void hello()
{
    cout << "hello from function" << endl;
}

int answer()
{
    return 42;
}

struct task_struct{
    void operator()()
    {
        cout << "hello from functor" << endl;
    }
};

int main()
{

    thread_pool tp(3);
    tp.add_task(&hello);
    tp.add_task( [] () { cout << "hello from lambda" << endl; });
    task_struct t;
    tp.add_task(t);
    future<int> res = tp.add_f_task<int>(answer);
    cout << "Answer is " << res.get() << endl;
    return 0;
}

