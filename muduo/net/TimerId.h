/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-06 09:54:42
 */
#ifndef NET_TIMERID_H
#define NET_TIMERID_H


class Timer;

///
/// An opaque identifier, for canceling Timer.
///
class TimerId
{
 public:
  explicit TimerId(Timer* timer = NULL, int64_t seq = 0)
    : timer_(timer),
    seq_(seq)
  {
  }

  // default copy-ctor, dtor and assignment are okay
  friend class TimerQueue;
 private:
  Timer* timer_;
  int64_t seq_;
};

#endif  // MUDUO_NET_TIMERID_H
