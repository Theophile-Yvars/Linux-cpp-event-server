#include "ThreadSafeQueue.hpp"

template <typename T>
void ThreadSafeQueue<T>::push(const T& item) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(item);
    cond_var_.notify_one();
}

template <typename T>
T ThreadSafeQueue<T>::pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        cond_var_.wait(lock);
    }
    T item = queue_.front();
    queue_.pop();
    return item;
}

