/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-07 19:19:49
 */
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/CurrentThread.h"

#include <stdio.h>
#include <unistd.h>

void runInThread()
{
  printf("runInThread(): pid = %d, tid = %d\n",
         getpid(), tid());
}

int main()
{
  printf("main(): pid = %d, tid = %d\n",
         getpid(), tid());

  EventLoopThread loopThread;
  EventLoop* loop = loopThread.startLoop();
  loop->runInLoop(runInThread);
  sleep(1);
  loop->runAfter(2, runInThread);
  sleep(3);
  loop->quit();

  printf("exit main().\n");
}

