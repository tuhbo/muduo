/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-08 16:38:20
 */
#include "muduo/base/Logging.h"
#include "muduo/net/Acceptor.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/SocketsOps.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/TcpConnection.h"

#include <functional>
#include <stdio.h>

TcpServer::TcpServer(EventLoop *loop, const InetAddress &listenAddr)
    : loop_(CHECK_NOTNULL(loop)),
    name_(listenAddr.toHostPort()),
    acceptor_(new Acceptor(loop, listenAddr)),
    started_(false),
    nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(
        std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2)
    );
}

TcpServer::~TcpServer() {}

void TcpServer::start() {
    if (!started_) {
        started_ = true;
    }
    if (!acceptor_->listenning()) {
        loop_->runInLoop(
            std::bind(&Acceptor::listen, acceptor_.get())
        );
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr) {
    loop_->assertInLoopThread();
    char buf[32];
    snprintf(buf, sizeof buf, "#%d", nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;

    LOG_INFO << "TcpServer::newConnection [" << name_
            << "] - new Connection [" << connName
            << "] from " << peerAddr.toHostPort();
    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn(
        new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->connectEstablished();
}