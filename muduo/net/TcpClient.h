/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-10 22:24:39
 */
#ifndef NET_TCPCLIENT_H
#define NET_TCPCLIENT_H

#include "muduo/net/TcpConnection.h"
#include "muduo/base/noncopyable.h"
#include "muduo/net/Connector.h"
#include <mutex>

class TcpClient : noncopyable {
    public:
        TcpClient(EventLoop* loop,
                    const InetAddress& serverAddr);
        ~TcpClient();  // force out-line dtor, for std::unique_ptr members.

        void connect();
        void disconnect();
        void stop();

        TcpConnectionPtr connection() const {
            std::unique_lock<std::mutex> lk(mutex_);
            return connection_;
        }

        EventLoop* getLoop() const { return loop_; }
        bool retry() const { return retry_; }
        void enableRetry() { retry_ = true; }

        // Set connection callback.
        // Not thread safe.
        void setConnectionCallback(ConnectionCallback &cb) {
            connectionCallback_ = std::move(cb); 
        }

        // Set message callback.
        // Not thread safe.
        void setMessageCallback(MessageCallback &cb) {
            messageCallback_ = std::move(cb);
        }

        // Set write complete callback.
        // Not thread safe.
        void setWriteCompleteCallback(WriteCompleteCallback &cb) {
            writeCompleteCallback_ = std::move(cb);
        }

    private:
        // Not thread safe, but in loop
        void newConnection(int sockfd);
        // Not thread safe, but in loop
        void removeConnection(const TcpConnectionPtr& conn);

        EventLoop* loop_;
        ConnectorPtr connector_; // avoid revealing Connector
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        WriteCompleteCallback writeCompleteCallback_;
        bool retry_;   // atomic
        bool connect_; // atomic
        // always in loop thread
        int nextConnId_;
        mutable std::mutex mutex_;
        TcpConnectionPtr connection_;
};
#endif