#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <functional>
#include <vector>

using namespace std;

int answer()
{
    this_thread::sleep_for(chrono::milliseconds(2500));
    return 42;
}

int main()
{
    vector<future<int>> futures;
    futures.push_back(async(launch::async, answer));
    futures.push_back(async(launch::async, answer));
    // join
    for (auto& fut : futures)
        cout << "The Answer: " << fut.get() << endl;
}
