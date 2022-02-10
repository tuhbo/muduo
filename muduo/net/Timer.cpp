/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-10 10:08:40
 */
#include "muduo/net/Timer.h"

std::atomic_int Timer::s_numCreated_(0);

void Timer::restart(Timestamp now)
{
  if (repeat_)
  {
    expiration_ = addTime(now, interval_);
  }
  else
  {
    expiration_ = Timestamp::invalid();
  }
}
