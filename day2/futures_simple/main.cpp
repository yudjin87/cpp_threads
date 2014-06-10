#include <iostream>
#include <thread>
#include <future>

using namespace std;

int answer(int id)
{
    this_thread::sleep_for(chrono::seconds(2));
    cout << "answer id " << id << endl;
    return 42;
}

int main()
{
    cout << "Hello World!" << endl;

//    packaged_task<int(void)> task(&answer);
//    future<int> result = task.get_future();
//    thread th1(move(task));
//    th1.detach();

    //future<int> result = async(launch::async, [] () { return answer(10);} );
    future<int> result = async(launch::async, answer, 10 );
    //result.wait();
    cout << "Answer is = " << result.get() << endl;

    return 0;
}

