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
class EventLoopThreadPool;

class TcpServer : noncopyable {
    public:
        TcpServer(EventLoop *loop, const InetAddress &listenAddr);
        ~TcpServer();

        // Set the number of threads for handling input.
        //
        // Always accepts new connection in loop's thread.
        // Must be called before @c start
        // @param numThreads
        // - 0 means all I/O in loop's thread, no thread will created.
        //   this is the default value.
        // - 1 means all I/O in another thread.
        // - N means a thread pool with N threads, new connections
        //   are assigned on a round-robin basis.
        void setThreadNum(int numThreads);

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

        // set write complete callback
        // Not thread safe
        void setWriteCompleteCallback(const WriteCompleteCallback& cb) {
            writeCompleteCallback_ = cb;
        }
    private:
        // Not thread safe, but in loop
        void newConnection(int sockfd, const InetAddress &peerAddr);
        // thread safe
        void removeConnection(const TcpConnectionPtr &conn);
        // not thread safe but in loop
        void removeConnectionInLoop(const TcpConnectionPtr &conn);

        typedef std::unordered_map<std::string, TcpConnectionPtr> ConnectionMap;

        EventLoop *loop_; // the acceptor loop;
        const std::string name_;
        std::unique_ptr<Acceptor> acceptor_;
        std::unique_ptr<EventLoopThreadPool> threadPool_;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        WriteCompleteCallback writeCompleteCallback_;
        bool started_;
        int nextConnId_; // always in loop thread
        ConnectionMap connections_;
};

#endif