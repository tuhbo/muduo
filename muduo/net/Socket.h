/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-08 09:54:25
 */
#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "muduo/base/noncopyable.h"

class InetAddress;

class Socket : noncopyable {
    public:
        explicit Socket(int sockfd)
            : sockfd_(sockfd)
        {}

        ~Socket();

        int fd() const { return sockfd_; }

        // abort if address in use
        void bindAddress(const InetAddress &localaddr);
        void listen();

        // On success, returns a non-negative interge for a desciptor 
        int accept(InetAddress *peeraddr);

        void setReuseAddr(bool on);

        void shutdownWrite();
    private:
        const int sockfd_;
};
#endif