#include <iostream>
#include <thread>
#include <string>

using namespace std;

void hello(int id, const string& msg)
{
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "Hello from thread id: " << id << endl;
    cout << "Message: " << msg << endl;
}

void mutilate(int& a)
{
    a = a * 10;
}

class Functor
{
    int id;
    int answer_;
public:
    Functor(int id) : id(id), answer_(-1) {}
    Functor() : id(99), answer_(-1) {}
    void operator()()
    {
        cout << "From functor " << id << endl;
        answer_ = 100;
    }
    int answer()
    {
        return answer_;
    }
};

thread launch_thread(int id)
{
    return thread( [] () { cout << "launched thread" << endl;});
}

int main()
{
    cout << "Hello different things!" << endl;
    thread th1(&hello, 10, "Medium is the message");
    int number{10};
    thread th2(&mutilate, ref(number));
    th1.join();

    // moving thread object
    thread copy(move(th2));  // th2 is no longer joinable (actually is not valid)
    copy.join();

    thread th3( [number] () { cout << "Hello from lambda: " << number << endl;} );
    th3.join();

    Functor f{99};
    //thread th4(ref(f));
    thread th4(f);
    th4.join();
    cout << f.answer() << endl;

    thread other = launch_thread(10); // move constructor also
    other.join();

    cout << "Number = " << number << endl;
    return 0;
}

