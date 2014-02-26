#include <iostream>
#include <thread>

using namespace std;

void hello()
{
    cout << "Hello World" << endl;
}

int main()
{
    thread th1(hello);
    th1.join();
}
