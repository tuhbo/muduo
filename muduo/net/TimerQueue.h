/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-06 09:48:17
 */
#ifndef NET_TIMERQUEUE_H
#define NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include "muduo/base/noncopyable.h"
#include "muduo/base/Timestamp.h"
#include "muduo/net/Callbacks.h"
#include "muduo/net/Channel.h"


class EventLoop;
class Timer;
class TimerId;


class TimerQueue : noncopyable {
  public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();

    // Must be thread safe. Usually be called from other threads.
    TimerId addTimer(const TimerCallback& cb,
                    Timestamp when,
                    double interval);

    void cancel(TimerId timerId);

  private:

    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;
    typedef std::pair<Timer*, int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;
    
    void addTimerInLoop(Timer *timer);
    void cancelInLoop(TimerId timerId);
    // called when timerfd alarms
    void handleRead();
    // move out all expired timers
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop* loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    // Timer list sorted by expiration
    TimerList timers_;

    // for cancel()
    bool callingExpiredTimers_;
    ActiveTimerSet activeTimers_;
    ActiveTimerSet cancelingTimers_;
};

#endif 
