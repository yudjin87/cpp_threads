#include <iostream>
#include <thread>

using namespace std;

void hello()
{
    this_thread::sleep_for(chrono::milliseconds(2500));
    cout << "Hello from thread" << endl;
}

int main()
{
    thread th1(hello);
    thread th2(hello);
    thread th3(hello);
    cout << "Hello from main thread!" << endl;
    th1.join();
    th2.join();
    th3.join();
    return 0;
}

