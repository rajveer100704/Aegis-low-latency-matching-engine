#pragma once
#include <queue>
#include <mutex>

template <typename T>
class ThreadSafeQueue {
public:
    void push(T item) {
        std::lock_guard<std::mutex> lock(m_);
        q_.push(std::move(item));
    }

    bool pop(T& item) {
        std::lock_guard<std::mutex> lock(m_);
        if (q_.empty()) return false;
        item = std::move(q_.front());
        q_.pop();
        return true;
    }

private:
    std::mutex m_;
    std::queue<T> q_;
};
