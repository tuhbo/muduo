/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-07 19:54:29
 */
#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H

#include "muduo/base/noncopyable.h"
#include "muduo/net/Channel.h"
#include "muduo/net/Socket.h"

#include <functional>

class EventLoop;
class InetAddress;

/**
 * @brief Acceptor of incoming TCP connections.
 * 
 */
class Acceptor : noncopyable {
    public:
        typedef std::function<void (int sockfd, 
                                const InetAddress &)> NewConnectionCallback;
        Acceptor(EventLoop *loop, const InetAddress &listenAddr);

        void setNewConnectionCallback(const NewConnectionCallback &cb) {
            newConnectionCallback_ = std::move(cb);
        }

        bool listenning() const { return listenning_; }
        void listen();

    private:
        void handleRead();

        EventLoop *loop_;
        Socket acceptSocket_;
        Channel acceptChannel_;
        NewConnectionCallback newConnectionCallback_;
        bool listenning_;
};
#endif