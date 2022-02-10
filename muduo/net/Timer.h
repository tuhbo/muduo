/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-06 09:55:43
 */
#ifndef MUDUO_NET_TIMER_H
#define MUDUO_NET_TIMER_H

#include "muduo/base/noncopyable.h"
#include "muduo/base/Timestamp.h"
#include "muduo/net/Callbacks.h"

#include <atomic>
//
// Internal class for timer event.
//
class Timer : noncopyable {
  public:
    Timer(const TimerCallback& cb, Timestamp when, double interval)
      : callback_(cb),
        expiration_(when),
        interval_(interval),
        repeat_(interval > 0.0),
        sequence_(++s_numCreated_)
    { }

    void run() const
    {
      callback_();
    }

    Timestamp expiration() const  { return expiration_; }
    bool repeat() const { return repeat_; }
    int64_t sequence() const { return sequence_; }

    void restart(Timestamp now);

  private:
    const TimerCallback callback_;
    Timestamp expiration_;
    const double interval_;
    const bool repeat_;
    const int64_t sequence_;
    static std::atomic_int s_numCreated_;
};

#endif  // MUDUO_NET_TIMER_H
