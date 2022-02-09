/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-08 16:23:12
 */
#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

#include "muduo/base/noncopyable.h"
#include "muduo/net/Callbacks.h"
#include "muduo/net/InetAddress.h"

#include <unordered_map>
#include <memory>
#include <string>

class EventLoop;
class Acceptor;

class TcpServer : noncopyable {
    public:
        TcpServer(EventLoop *loop, const InetAddress &listenAddr);
        ~TcpServer();

        /**
         * @brief Starts the server is it's not listenning.
         * It's harmless to call it multiple times.
         */
        void start();

        // Not thread safe
        void setConnectionCallback(const ConnectionCallback &cb) {
            connectionCallback_ = cb;
        }

        // not thread safe
        void setMessageCallback(const MessageCallback &cb) {
            messageCallback_ = cb;
        }
    private:
        void newConnection(int sockfd, const InetAddress &peerAddr);

        typedef std::unordered_map<std::string, TcpConnectionPtr> ConnectionMap;

        EventLoop *loop_; // the acceptor loop;
        const std::string name_;
        std::unique_ptr<Acceptor> acceptor_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        bool started_;
        int nextConnId_; // always in loop thread
        ConnectionMap connections_;
};

#endif