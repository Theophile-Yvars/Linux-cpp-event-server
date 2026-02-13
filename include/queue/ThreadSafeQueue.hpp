#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    void push(const T& item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(item);
        }
        cond_var_.notify_one();
        std::cout<<"PUSH : "<< std::endl;
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);

        cond_var_.wait(lock, [this] {
            return !queue_.empty();
        });

        T item = queue_.front();
        queue_.pop();
        std::cout<<"POP : "<< std::endl;
        return item;
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};