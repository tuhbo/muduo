/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-02 16:34:24
 */
#include "muduo/net/EventLoop.h"
#include "muduo/base/Logging.h"
#include "muduo/net/Poller.h"
#include "muduo/net/Channel.h"
#include "muduo/net/TimerQueue.h"
#include <poll.h>

__thread EventLoop *t_loopInthisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop()
    : looping_(false),
    quit_(false),
    threadId_(::tid()),
    poller_(new Poller(this)),
    timerQueue_(new TimerQueue(this))
{
    LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;
    if (t_loopInthisThread) {
        LOG_FATAL << "Another EventLoop " << t_loopInthisThread
                << " exists in this thread " << threadId_;
    } else {
        t_loopInthisThread = this;
    }
}

EventLoop::~EventLoop() {
    assert(!looping_);
    t_loopInthisThread = NULL;
}

void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;

    while (!quit_) {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        for (ChannelList::iterator it = activeChannels_.begin();
            it != activeChannels_.end(); ++it)
        {
            (*it)->handleEvent();
        }
    }

    LOG_TRACE << "EvnetLoop " << this << " top looping";
    looping_ = false;
}

void EventLoop::quit() {
    quit_ = false;
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb) {
    return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb) {
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb) {
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(cb, time, interval);
}

void EventLoop::updateChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::abortNotInLoopThread() {
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << ", current thread id = " <<  ::tid();
}

