/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-08 11:16:04
 */
#include "muduo/net/Acceptor.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/SocketsOps.h"
#include "muduo/base/Timestamp.h"
#include <stdio.h>

void newConnection(int sockfd, const InetAddress& peerAddr)
{
  std::string tmp = Timestamp::now().toFormattedString(true);
  tmp += "\n";
  printf("newConnection(): accepted a new connection from %s\n",
         peerAddr.toHostPort().c_str());
  int len = tmp.size();
  ::write(sockfd, tmp.c_str(), len);
  sockets::close(sockfd);
}

int main()
{
  printf("main(): pid = %d\n", getpid());

  InetAddress listenAddr(9981);
  EventLoop loop;

  Acceptor acceptor(&loop, listenAddr);
  acceptor.setNewConnectionCallback(newConnection);
  acceptor.listen();
  loop.loop();
}
