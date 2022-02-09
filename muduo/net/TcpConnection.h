/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-08 11:58:54
 */
#ifndef NET_TCPCONNECTION_H
#define NET_TCPCONNECTION_H

#include "muduo/net/Callbacks.h"
#include "muduo/net/InetAddress.h"
#include "muduo/base/noncopyable.h"

#include <string>

class Channel;
class EventLoop;
class Socket;

/**
 * @brief TCP connection, for both client and server usage.
 */
class TcpConnection : noncopyable,
                    public std::enable_shared_from_this<TcpConnection>
{
    public:
        TcpConnection(EventLoop *loop, 
                    const std::string &name,
                    int sockfd,
                    const InetAddress &localAddr,
                    const InetAddress &peerAddr);
        
        ~TcpConnection();

        EventLoop *getLoop() const { return loop_; }

        const std::string& name() const { return name_; }
        const InetAddress& localAddress() { return localAddr_; }
        const InetAddress& peerAddress() { return peerAddr_; }
        bool connected() const { return state_ == KConnected; }

        void setConnectionCallback(const ConnectionCallback& cb) {
            connectionCallback_ = cb; 
        }

        void setMessageCallback(const MessageCallback& cb) {
            messageCallback_ = cb; 
        }

        // Internal use only.
        // called when TcpServer accepts a new connection
        void connectEstablished();   // should be called only once
    private:
        enum StateE { KConnecting, KConnected,};

        void setState(StateE s) { state_ = s; }
        void handleRead();
        
        EventLoop *loop_;
        std::string name_;
        StateE state_;
        std::unique_ptr<Socket> socket_;
        std::unique_ptr<Channel> channel_;
        InetAddress localAddr_;
        InetAddress peerAddr_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
};

#endif