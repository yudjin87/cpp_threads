#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

class BankAccount
{
    const int id_;
    double balance_;
    mutex mtx;
public:
    BankAccount(int id, double balance)
        : id_(id), balance_(balance)
    {
    }

    void print()
    {
        cout << "BA: " << id_;
        cout << " balance = " << balance_ << endl;
    }

    void transfer(BankAccount& to, double amount)
    {
        unique_lock<mutex> lock_myself(mtx, defer_lock);
        unique_lock<mutex> lock_other(to.mtx, defer_lock);
        lock(lock_myself, lock_other);
        balance_ -= amount;
        to.balance_ += amount;
    }
};

void test(BankAccount* from, BankAccount* to)
{
    for (int i = 0 ; i < 10000 ; ++i)
    {
        //cout << "." << flush;
        from->transfer(*to, 1.0);
    }
}

int main()
{
    BankAccount acc1(1, 20000);
    BankAccount acc2(2, 20000);
    thread th1(test, &acc1, &acc2);
    thread th2(test, &acc2, &acc1);
    thread th3(test, &acc2, &acc1);
    th1.join();
    th2.join();
    th3.join();
    acc1.print();
    acc2.print();
    return 0;
}

