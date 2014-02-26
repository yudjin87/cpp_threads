#include <iostream>
#include <thread>
//#include <boost/thread.hpp>
#include <functional>
//#include <boost/chrono.hpp>
#include <chrono>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

class Hello
{
    string msg_;
public:
    Hello(string message) : msg_(message)
    {

    }

    void operator()()
    {
        cout << "Hello World from " << msg_ << endl;
    }
};

void hello(string msg)
{
    cout << "Hello World from " << msg << endl;
}

thread make_thread(string msg)
{
    //Hello h = Hello();
    //thread th1{ Hello("Leszek") };
    //string msg = "Leszek";
    thread th1(hello, msg);
    //thread th1( [msg]() { hello(msg); });
//    int a = 10;
//    std::function<void(void)> hello = [a](){
//        this_thread::sleep_for(chrono::seconds(1));
//        cout << "Hello world, " << a << endl;
//    };
//    thread th1{ hello };
    //boost::this_thread::sleep_for(boost::chrono::seconds(1));
    return th1;
}

int main()
{
    cout << thread::hardware_concurrency() << endl;
    {
        vector<thread> thg;
        //thread_group thg;
        for (int i; i < 4 ; ++i)
        {
            string msg = "Thread ";
            msg += lexical_cast<string>(i);
            thg.emplace_back (hello, msg);
//            //thread* th = new thread(hello, msg);
//            //thg.add_thread(th);
//            thg.create_thread( bind(hello, msg) );
        }
        for ( thread& th : thg)
        {
            th.join();
        }
//        thg.join_all();
    }
    //th2.join();
    //this_thread::sleep_for(chrono::seconds(2));
}
