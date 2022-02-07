/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-29 22:28:03
 */
#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include "muduo/base/noncopyable.h"

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <unistd.h>
#include <string>
#include <atomic>

class Thread:noncopyable {
    public:
        typedef std::function<void()> ThreadFunc;

        Thread(ThreadFunc func, const std::string &name = std::string());
        ~Thread();
        
        void start();
        void join();

        bool started() const { return started_; }
        pid_t tid() const { return tid_; }
        const std::string& name() const { return name_; }
        static int numCreated() { return numCreated_; }
    private:
        void setDefaultName();

        bool started_;
        bool joined_;
        std::thread thread_;
        pid_t tid_;
        ThreadFunc func_;
        std::string name_;
        static std::atomic_int numCreated_;
        mutable std::mutex mutex_;
        std::condition_variable cond_;
};

#endif