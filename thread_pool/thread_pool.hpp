#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

class thread_pool {
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<std::function<void()>> task_queue_;
    std::vector<std::thread> threads_;
    bool stop_;
public:
    thread_pool(size_t num);
    ~thread_pool();

    void enqueue(std::function<void()> f);

    void stop();
};

inline thread_pool::thread_pool(size_t num): 
     stop_(false) {
    for (auto i  = 0; i < num; ++i) {
        threads_.emplace_back([&] {
            for (;;)
            {
                std::unique_lock<std::mutex> lock(mutex_);

                while (task_queue_.empty())
                    condition_.wait(lock);
              
                if (stop_ == true && task_queue_.empty()) 
                    return;

                auto task = task_queue_.front();
                task_queue_.pop();
                task();
            }
        });
    }
}

inline void thread_pool::enqueue(std::function<void()> f) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        task_queue_.push(std::move(f));
    }
    condition_.notify_one();
}

inline void thread_pool::stop() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    
    for (auto& i : threads_) 
        i.join();
        
}

inline thread_pool::~thread_pool() {
   stop();
}



