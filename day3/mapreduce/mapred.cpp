#include <iostream>
#include <thread>
#include <boost/filesystem.hpp>
#include "thread_safe_queue.hpp"

ThreadSafeQueue<boost::filesystem::path> in_queue;
ThreadSafeQueue<long> out_queue;

void producer(std::string &&start )
{
    boost::filesystem::recursive_directory_iterator it(start);
    boost::filesystem::recursive_directory_iterator end;
    for ( ; it != end ; ++it)
    {
        if (boost::filesystem::is_regular_file(*it))
        {
            if(boost::filesystem::extension(*it) == ".cpp")
            {
                in_queue.push(*it);
            }
        }
    }
    in_queue.push( boost::filesystem::path() );
}

void worker()
{
    while(true)
    {
        boost::filesystem::path path;
        in_queue.pop(path);
        if (path.string() == "")
        {
            in_queue.push(path);
            break;
        }
        out_queue.push( boost::filesystem::file_size(path) );
    }
}

void mapper(int n_of_threads)
{
    std::vector<std::thread> threads;
    for (int i = 0 ; i < n_of_threads ; ++i)
    {
        threads.emplace_back( worker);
    }
    for (auto & thd : threads)
    {
        thd.join();
    }
    out_queue.push(-1);
}

long reducer()
{
    long total_size = 0;
    while(true)
    {
        long size;
        out_queue.pop(size);
        if (size == -1)
        {
            break;
        }
        total_size += size;
    }
    return total_size;
}

int main()
{
    std::thread prod_thread(producer, "/home/leszek/");
    std::thread mapper_thread(mapper, 4);
    //
    std::cout  << "Total: " << reducer() << std::endl;
    mapper_thread.join();
    prod_thread.join();
}
