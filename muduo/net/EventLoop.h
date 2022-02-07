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
#include <functional>
#include <mutex>

class Channel;
class Poller;
class TimerQueue;

class EventLoop : noncopyable {
    public:
        typedef std::function<void()> Functor;
        EventLoop();
        ~EventLoop();

        void quit();

        // Time when poll returns, usually means data arrivial.
        //
        Timestamp pollReturnTime() const { return pollReturnTime_; }
        /**
         * @brief Runs callback immediately in the loop thread
         * It wakes up the loop, and run the cb.
         * If in the same loop thread, cb is run within the function.
         * 
         * @param cb 
         */
        void runInLoop(const Functor &cb);
        
        /**
         * @brief Queues callback in the loop thread.
         * Runs after finish pooling.
         * Safe to call from other threads.
         * @param cb 
         */
        void queueInLoop(const Functor &cb);

        // timers
        // Runs callback at 'time'.
        TimerId runAt(const Timestamp& time, const TimerCallback& cb);
        // Runs callback after @c delay seconds.
        TimerId runAfter(double delay, const TimerCallback& cb);
        //Runs callback every @c interval seconds.
        TimerId runEvery(double interval, const TimerCallback& cb);

        void cancel(TimerId timerId);

        void loop();

        void wakeup();
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
        void handleRead(); // for waked up
        void doPendingFunctors();

        typedef std::vector<Channel *> ChannelList;

        bool looping_;
        bool quit_;
        bool callingPendingFunctors_;
        const pid_t threadId_;
        Timestamp pollReturnTime_;
        std::unique_ptr<Poller> poller_;
        std::unique_ptr<TimerQueue> timerQueue_;
        int wakeupFd_;
        std::unique_ptr<Channel> wakeupChannel_;
        ChannelList activeChannels_;
        std::mutex mutex_;
        std::vector<Functor> pendingFunctors_;
};

#endif