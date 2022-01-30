/*
 * @Description: thread pool
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-29 23:20:54
 */
#ifndef BASE_THREADPOOL_H
#define BASE_THREADPOOL_H

#include "muduo/base/noncopyable.h"
#include "muduo/base/Thread.h"
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <deque>

class ThreadPool : noncopyable {
    public:
        typedef std::function<void ()> Task;

        explicit ThreadPool(const std::string &nameArg = std::string("ThreadPool"));
        ~ThreadPool();

        // must be called before start()

        void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }

        void setThreadInitCallback(const Task &cb) { threadInitCallback_ = cb; }

        void start(int numThreads);

        void stop();

        const std::string &name() const {
            return name_;
        }

        size_t queueSize() const;

        void run(Task f);
    private:
        bool isFull() const;
        void runInThread();

        Task take();

        mutable std::mutex mutex_;
        std::condition_variable notFull_;
        std::condition_variable notEmpty_;

        std::string name_;

        Task threadInitCallback_;
        std::vector<std::unique_ptr<Thread>> threads_;
        std::deque<Task> queue_;
        size_t maxQueueSize_;
        bool running_;
        
};
#endif 