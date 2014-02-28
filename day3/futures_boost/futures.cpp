#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

using namespace std;
using namespace boost;
int answer()
{
    this_thread::sleep_for(chrono::milliseconds(2500));
    return 42;
}

int main()
{
    packaged_task<int> ptask(answer);
    unique_future<int> future_result = ptask.get_future();
    thread th1(boost::move(ptask));
    //ptask();
    cout << "Is ready?" << future_result.is_ready() << endl;
    cout << "The Answer: " << future_result.get() << endl;
}
