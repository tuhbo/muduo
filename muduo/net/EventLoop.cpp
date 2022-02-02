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
#include <poll.h>

__thread EventLoop *t_loopInthisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop()
    : looping_(false),
    quit_(false),
    threadId_(::tid()),
    poller_(new Poller(this))
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
        poller_->poll(kPollTimeMs, &activeChannels_);
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

