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
#include "muduo/net/Buffer.h"
#include "muduo/base/Timestamp.h"

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

        // Thread safe
        void send(const void *message, size_t len);
        // Thread safe
        void send(const std::string &message);
        // Thread safe
        void shutdown();
        void setTcpNoDelay(bool on);

        void setConnectionCallback(const ConnectionCallback& cb) {
            connectionCallback_ = cb; 
        }

        void setMessageCallback(const MessageCallback& cb) {
            messageCallback_ = cb; 
        }

        void setCloseCallback(const CloseCallback &cb) {
            closeCallback_ = cb;
        }

        void setWriteCompleteCallback(const WriteCompleteCallback &cb) {
            writeCompleteCallback_ = cb;
        }

        // Internal use only.
        // called when TcpServer accepts a new connection
        void connectEstablished();   // should be called only once
        
        // called when TcpServer has removed me from its map
        void connectDestroyed();
    private:
        enum StateE { KConnecting, KConnected, KDisconnected, KDisconnecting, };

        void setState(StateE s) { state_ = s; }
        void handleRead(Timestamp receiveTime);
        void handleWrite();
        void handleClose();
        void handleError();
        void sendInLoop(const std::string &message);
        void shutdownInLoop();
        
        EventLoop *loop_;
        std::string name_;
        StateE state_;
        std::unique_ptr<Socket> socket_;
        std::unique_ptr<Channel> channel_;
        InetAddress localAddr_;
        InetAddress peerAddr_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        WriteCompleteCallback writeCompleteCallback_;
        CloseCallback closeCallback_;
        Buffer inputBuffer_;
        Buffer outputBuffer_;
};

#endif