/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-07 19:53:27
 */
#include "muduo/base/Logging.h"
#include "muduo/net/Acceptor.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/SocketsOps.h"

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr)
    : loop_(loop),
    acceptSocket_(sockets::createNonblockingOrDie()),
    acceptChannel_(loop, acceptSocket_.fd()),
    listenning_(false)
{
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(
        std::bind(&Acceptor::handleRead, this)
    );
}

void Acceptor::listen() {
    loop_->assertInLoopThread();
    listenning_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}

void Acceptor::handleRead() {
    loop_->assertInLoopThread();
    InetAddress peerAddr(0);
    int connfd = acceptSocket_.accept(&peerAddr);
    if (connfd >= 0) {
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);
        } else {
            sockets::close(connfd);
        }
    }
}

