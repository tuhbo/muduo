/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-09 22:24:13
 */
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/CurrentThread.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

void print(EventLoop* p = NULL)
{
  printf("main(): pid = %d, tid = %d, loop = %p\n",
         getpid(), tid(), p);
}

void init(EventLoop* p)
{
  printf("(): pid = %d, tid = %d, loop = %p\n",
         getpid(), tid(), p);
}

int main()
{
  print();

  EventLoop loop;
  loop.runAfter(11, std::bind(&EventLoop::quit, &loop));

  {
    printf("Single thread %p:\n", &loop);
    EventLoopThreadPool model(&loop);
    model.setThreadNum(0);
    model.start();
    assert(model.getNextLoop() == &loop);
    assert(model.getNextLoop() == &loop);
    assert(model.getNextLoop() == &loop);
  }

  {
    printf("Another thread:\n");
    EventLoopThreadPool model(&loop);
    model.setThreadNum(1);
    model.start();
    EventLoop* nextLoop = model.getNextLoop();
    nextLoop->runAfter(2, std::bind(print, nextLoop));
    assert(nextLoop != &loop);
    assert(nextLoop == model.getNextLoop());
    assert(nextLoop == model.getNextLoop());
    ::sleep(3);
  }

  {
    printf("Three threads:\n");
    EventLoopThreadPool model(&loop);
    model.setThreadNum(3);
    model.start();
    EventLoop* nextLoop = model.getNextLoop();
    nextLoop->runInLoop(std::bind(print, nextLoop));
    assert(nextLoop != &loop);
    assert(nextLoop != model.getNextLoop());
    assert(nextLoop != model.getNextLoop());
    assert(nextLoop == model.getNextLoop());
  }

  loop.loop();
}


