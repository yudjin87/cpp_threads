#include <boost/thread.hpp>
#include <iostream>

class BankAccount
{
    const int id_;
    double balance_;
    mutable boost::mutex mtx_;
public:
    BankAccount(int id, double balance) : id_(id), balance_(balance)
    {
    }

    void print()
    {
        std::cout << "Bank Account " << id_ << std::endl;
        boost::lock_guard<boost::mutex> guard(mtx_);
        std::cout << "balance = " << balance_ << std::endl;
    }

    void transfer(BankAccount& to, double amount)
    {
        boost::unique_lock<boost::mutex> local_lk(mtx_, boost::defer_lock);
        boost::unique_lock<boost::mutex> target_lk(to.mtx_, boost::defer_lock);
        boost::lock(local_lk, target_lk);
        balance_ -= amount;
        to.balance_ += amount;
    }
};

void deadlock_test(BankAccount* from, BankAccount* to)
{
    for (int i = 0 ; i < 10000; ++i)
    {
        std::cout << ".";
        from->transfer(*to, 1.0);
    }
}

int main()
{
    BankAccount ba1(1, 10000);
    BankAccount ba2(2, 10000);

    boost::thread tr1(deadlock_test, &ba1, &ba2 );
    boost::thread tr2(deadlock_test, &ba2, &ba1 );

    tr1.join();
    tr2.join();

    ba1.print();
    ba2.print();
}
