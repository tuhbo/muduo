/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-07 21:52:34
 */
#ifndef NET_INETADDRESS_H
#define NET_INETADDRESS_H

#include <string>
#include <netinet/in.h>

class InetAddress {
    public:

        // Constructs an endpoint with given port number.
        // Mostly used in TcpServer listening.
        explicit InetAddress(uint16_t port);

        // Constructs an endpoint with given ip and port.
        // @c ip should be "1.2.3.4"
        InetAddress(const std::string &ip, uint16_t port);

        InetAddress(const struct sockaddr_in& addr)
            : addr_(addr)
        { }
        
        std::string toHostPort() const;

        const struct sockaddr_in& getSockAddrInet() const { return addr_; }
        void setSockAddrInet(const struct sockaddr_in& addr) { addr_ = addr; }
    private:
        struct sockaddr_in addr_;
};
#endif