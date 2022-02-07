/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-07 17:14:13
 */
#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

#include "muduo/base/Thread.h"
#include "muduo/base/noncopyable.h"
#include <mutex>
#include <condition_variable>

class EventLoop;
class EventLoopThread : noncopyable {
    public:
        EventLoopThread();
        ~EventLoopThread();
        EventLoop *startLoop();
    private:
        void threadFunc();

        EventLoop *loop_;
        bool exiting_;
        Thread thread_;
        std::mutex mutex_;
        std::condition_variable cond_;
};
#endif