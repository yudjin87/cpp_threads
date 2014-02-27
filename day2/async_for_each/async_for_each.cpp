#include <vector>
#include <boost/thread.hpp>
#include <functional>
#include <chrono>
#include <iostream>

template <typename OUT, typename IN>
std::vector<OUT> async_for_each(const std::vector<IN> &in,
                                std::function<OUT (IN)> f)
{
    std::vector<OUT> res(in.size());
    boost::thread_group workers;
    boost::mutex mtx;
    size_t cnt = 0;

    // create workers
    for (int i = 0 ; i < boost::thread::hardware_concurrency() ; ++i)
    {
        workers.create_thread( [&]() {
            while(true)
            {
                size_t idx;
                {
                    boost::lock_guard<boost::mutex> lock(mtx);
                    if (cnt == in.size()) {
                        break;
                    }
                    idx = cnt;
                    ++cnt;
                }
                res[idx] = f(in[idx]);
            }
        });
    }

    workers.join_all();
    return res;
}

template <typename OUT, typename IN>
std::vector<OUT> sync_for_each(const std::vector<IN> &in,
                               std::function<OUT (IN)> f)
{
    std::vector<OUT> res(in.size());


    for(int i = 0 ; i < in.size() ; ++i)
    {
        res[i] = f(in[i]);
    }
    return res;
}

bool is_prime(int num)
{
    for (int div = 2 ; div <= sqrt((double)num) ; ++div)
        if (num % div == 0)
            return false;
    return true;
}

int main()
{
    std::vector<int> in {2,3,4,5,6,7,8,9,10};
    for (int i = 1000000 ; i < 1100000 ; ++i)
        in.push_back(i);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<bool> res = sync_for_each<bool, int>(in, is_prime);
    auto end = std::chrono::high_resolution_clock::now();
    float seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "   " << seconds << " msec" << std::endl;

    auto i_in = in.cbegin();
    auto i_res = res.cbegin();
//    for (; i_in != in.cend() ; ++i_in, ++i_res)
//    {
//        if (*i_res)
//            std::cout << *i_in << ", ";
//    }
//    std::cout << std::endl;
}
