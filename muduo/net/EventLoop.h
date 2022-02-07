/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-02 16:05:35
 */
#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "muduo/base/Timestamp.h"
#include "muduo/base/CurrentThread.h"
#include "muduo/base/noncopyable.h"
#include "muduo/net/Callbacks.h"
#include "muduo/net/TimerId.h"

#include <vector>
#include <memory>

class Channel;
class Poller;
class TimerQueue;

class EventLoop : noncopyable {
    public:
        EventLoop();
        ~EventLoop();

        void quit();

        // Time when poll returns, usually means data arrivial.
        //
        Timestamp pollReturnTime() const { return pollReturnTime_; }

        // timers
        // Runs callback at 'time'.
        TimerId runAt(const Timestamp& time, const TimerCallback& cb);
        // Runs callback after @c delay seconds.
        TimerId runAfter(double delay, const TimerCallback& cb);
        //Runs callback every @c interval seconds.
        TimerId runEvery(double interval, const TimerCallback& cb);

        void cancel(TimerId timerId);

        void loop();

        void updateChannel(Channel* channel);

        void assertInLoopThread() {
            if (!isInLoopThread()) {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const {
            return threadId_ == ::tid();
        }
    private:
        void abortNotInLoopThread();

        typedef std::vector<Channel *> ChannelList;

        bool looping_;
        bool quit_;
        const pid_t threadId_;
        Timestamp pollReturnTime_;
        std::unique_ptr<Poller> poller_;
        std::unique_ptr<TimerQueue> timerQueue_;
        ChannelList activeChannels_;

};

#endif