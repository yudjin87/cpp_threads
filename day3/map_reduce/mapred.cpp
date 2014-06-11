#include <iostream>
#include <thread>
#include <string>
#include <future>
#include <boost/filesystem.hpp>
#include "thread_safe_queue.h"
#include "thread_pool.h"

using namespace std;

class ReduceFileSize
{
    std::string path_;
    std::string filter_;

    thread_safe_queue<boost::filesystem::path> input_queue_;
    thread_safe_queue<int64_t> output_queue_;
    size_t n_of_mappers_;
    std::promise<int64_t> promise_;
    thread_pool *tp;

    void producer()
    {
        //feeds input queue with matching file paths
        boost::filesystem::recursive_directory_iterator it(path_);
        boost::filesystem::recursive_directory_iterator end;
        for ( ; it != end ; ++it)
        {
            if(boost::filesystem::is_regular_file(*it))
            {
                if (boost::filesystem::extension(*it) == filter_)
                {
                    input_queue_.push(*it);
                }
            }
        }

        // end_of_work
        for ( int i = 0 ; i < n_of_mappers_ ; i++)
        {
            input_queue_.push( boost::filesystem::path() );
        }
    }

    void mapper()
    {
        // for file path in input queue puts file size in output queue
        while(true)
        {
            boost::filesystem::path path;
            input_queue_.pop(path);
            if (path.empty())
            {
                output_queue_.push(-1);
                return;
            }
            output_queue_.push(boost::filesystem::file_size(path));
        }
    }

    void reducer()
    {
        // sums file sizes from output queue
        int64_t total_size = 0;
        size_t stopper = 0;

        while(stopper < n_of_mappers_)
        {
            int64_t size = 0;
            output_queue_.pop(size);
            if (size == -1)
            {
                ++stopper;
            }
            else
            {                
                total_size += size;
                //std::cout << total_size << std::endl;
                //std::cout << suma << std::endl;
            }
        }
        promise_.set_value(total_size);
    }

public:
    ReduceFileSize(std::string path, std::string filter)
        : path_(path), filter_(filter)
    {
        n_of_mappers_ = std::thread::hardware_concurrency();
        if (n_of_mappers_ < 2)
            n_of_mappers_ = 2;
    }

    ~ReduceFileSize()
    {
        //threads_.join_all();
        delete tp;
    }

    std::future<int64_t> run_async()
    {
        tp = new thread_pool(n_of_mappers_ + 2);
        tp->add_task( [&](){ producer(); } );

        for (int i = 0 ; i < n_of_mappers_ ; ++i)
        {
            tp->add_task( [&](){ mapper(); });

        }
        tp->add_task( [&]() { reducer(); });

        return promise_.get_future();
    }
};

int main()
{
    ReduceFileSize reducer1("/home/leszek", ".cpp");
    std::future<int64_t> result1 = reducer1.run_async();
    // main thread

    std::cout << "Total file size = " << result1.get() << std::endl;
}

