#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include "thread_safe_queue.hpp"

class ThreadPool
{
private:
    ThreadSafeQueue<std::function<void(void)>> queue;
public:
    ThreadPool(int n_of_threads);
    ~ThreadPool();
    void add_task(std::function<void(void)>);
};
