/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-10 21:53:24
 */
#ifndef NET_CONNECTOR_H
#define NET_CONNECTOR_H
#include "muduo/base/noncopyable.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TimerId.h"

#include <functional>
#include <memory>

class EventLoop;
class Channel;

class Connector : noncopyable {
    public:
        typedef std::function<void (int sockfd)> NewConnectionCallback;

        Connector(EventLoop *loop, const InetAddress &serverAddr);
        ~Connector();

        void setNewConnectionCallback(const NewConnectionCallback &cb) {
            newConnectionCallback_ = std::move(cb);
        }

        void start(); // can be called in any thread
        void restart(); // must be called in loop thread
        void stop(); // can be called in any thread

        const InetAddress &serverAddress() const { return serverAddr_; }
    private:
        enum States {kDisconnected, kConnecting, kConnected};
        static const int kMaxRetryDelayMs = 30 * 1000;
        static const int kInitRetryDelayMs = 500;

        void setState(States s) { state_ = s; }
        void startInLoop();
        void connect();
        void connecting(int sockfd);
        void handleWrite();
        void handleError();
        void retry(int sockfd);
        int removeAndResetChannel();
        void resetChannel();

        EventLoop *loop_;
        InetAddress serverAddr_;
        bool connect_;
        States state_;
        std::unique_ptr<Channel> channel_;
        NewConnectionCallback newConnectionCallback_;
        int retryDelayMs_;
        TimerId timerId_;
};

typedef std::shared_ptr<Connector> ConnectorPtr;
#endif