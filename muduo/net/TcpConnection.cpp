/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-08 19:28:50
 */
#include "muduo/base/Logging.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/net/Socket.h"

#include <functional>
#include <errno.h>
#include <stdio.h>

TcpConnection::TcpConnection(EventLoop *loop,
                            const std::string &nameArg,
                            int sockfd,
                            const InetAddress &localAddr,
                            const InetAddress &peerAddr)
    : loop_(CHECK_NOTNULL(loop)),
    name_(nameArg),
    state_(KConnecting),
    socket_(new Socket(sockfd)),
    channel_(new Channel(loop, sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr)
{
    LOG_DEBUG << "TcpConnection::ctor[" << name_ << "] at " << this
                << " fd=" << sockfd;
    channel_->setReadCallback(
        std::bind(&TcpConnection::handleRead, this)
    );
}

TcpConnection::~TcpConnection() {
    LOG_DEBUG << "TcpConnection::dtor[" <<  name_ << "] at " << this
            << " fd=" << channel_->fd();
}

void TcpConnection::connectEstablished() {
    loop_->assertInLoopThread();
    assert(state_ == KConnecting);
    setState(KConnected);
    channel_->enableReading();

    connectionCallback_(shared_from_this());
}

void TcpConnection::handleRead() {
    char buf[65536];
    ssize_t n = ::read(channel_->fd(), buf, sizeof buf);
    messageCallback_(shared_from_this(), buf, n);
    // FIXME: close connection if n == 0
}

