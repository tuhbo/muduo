/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-09 21:36:51
 */

#ifndef NET_EVENTLOOPTHREADPOOL_H
#define NET_EVENTLOOPTHREADPOOL_H

#include "muduo/base/noncopyable.h"
#include "muduo/base/Thread.h"
#include <vector>
#include <functional>
#include <mutex>
#include <memory>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable {
    public:
        EventLoopThreadPool(EventLoop *baseLoop);
        ~EventLoopThreadPool();
        void setThreadNum(int numThreads) {
            numThreads_ = numThreads;
        }
        void start();
        EventLoop *getNextLoop();

    private:
        EventLoop *baseLoop_;
        bool started_;
        int numThreads_;
        int next_; // loop index
        std::vector<std::unique_ptr<EventLoopThread>> threads_;
        std::vector<EventLoop *>  loops_;
};
#endif