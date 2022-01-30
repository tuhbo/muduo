/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-29 22:55:32
 */
#include "muduo/base/Thread.h"
#include "muduo/base/CurrentThread.h"

std::atomic_int Thread::numCreated_(0);

Thread::Thread(ThreadFunc func, const std::string &name)
    : started_(false),
    joined_(false),
    tid_(0),
    func_(std::move(func)),
    name_(name),
    mutex_(),
    cond_()
{
    setDefaultName();
}

Thread::~Thread() {
    if (started_ && !joined_) {
        thread_->detach();
    }
}

void Thread::start() {
    started_ = true;
    thread_ = std::make_shared<std::thread>(
        std::thread([&](){
            tid_ = ::tid();
            cond_.notify_one();
            func_();
        })
    );

    std::unique_lock<std::mutex> lk(mutex_);
    cond_.wait(lk, [&](){ return tid_ != 0; });
}

void Thread::join() {
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName() {
    int num = ++numCreated_;
    if(name_.empty()) {
        char buf[32] = {0};
        snprintf(buf,sizeof buf,"Thread%d",num);
        name_ = buf;
    }
}