#pragma once
#include <atomic>
#include <vector>

template <typename T>
class LockFreeQueue {
public:
    explicit LockFreeQueue(size_t size)
        : buf_(size), cap_(size), head_(0), tail_(0) {}

    bool push(const T& item) {
        size_t t = tail_.load(std::memory_order_relaxed);
        size_t n = (t + 1) % cap_;
        if (n == head_.load(std::memory_order_acquire)) return false;
        buf_[t] = item;
        tail_.store(n, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        size_t h = head_.load(std::memory_order_relaxed);
        if (h == tail_.load(std::memory_order_acquire)) return false;
        item = buf_[h];
        head_.store((h + 1) % cap_, std::memory_order_release);
        return true;
    }

private:
    std::vector<T> buf_;
    size_t cap_;
    std::atomic<size_t> head_, tail_;
};
