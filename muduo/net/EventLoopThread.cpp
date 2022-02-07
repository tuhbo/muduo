/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-07 17:31:11
 */
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoop.h"
#include <assert.h>


EventLoopThread::EventLoopThread()
    : loop_(NULL),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::threadFunc, this)),
    mutex_(),
    cond_()
{
}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    loop_->quit();
    thread_.join();
}

EventLoop *EventLoopThread::startLoop() {
    assert(!thread_.started());
    thread_.start();
    {
        std::unique_lock<std::mutex> lk(mutex_);
        while (loop_ == NULL) {
            cond_.wait(lk);
        }
    }
    return loop_;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    {
        std::unique_lock<std::mutex> lk(mutex_);
        loop_ = &loop;
        cond_.notify_one();
    }
    loop.loop();
}