#pragma once
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>
template<class T> class SafeQueue {
public:
 void push(T value){{std::lock_guard<std::mutex> l(m_);q_.push(std::move(value));}cv_.notify_one();}
 bool pop(T& value,std::chrono::milliseconds timeout){std::unique_lock<std::mutex> l(m_);if(!cv_.wait_for(l,timeout,[this]{return !q_.empty();}))return false;value=std::move(q_.front());q_.pop();return true;}
 bool tryPop(T& value){std::lock_guard<std::mutex> l(m_);if(q_.empty())return false;value=std::move(q_.front());q_.pop();return true;}
private: std::mutex m_;std::condition_variable cv_;std::queue<T> q_;
};