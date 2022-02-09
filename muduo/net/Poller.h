/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-02 21:20:53
 */
#ifndef NET_POLLER_H
#define NET_POLLER_H

#include "muduo/base/noncopyable.h"
#include "muduo/base/Timestamp.h"
#include "muduo/net/EventLoop.h"
#include <map>
#include <vector>

class Channel;
struct pollfd;

class Poller : noncopyable {
    public:
        typedef std::vector<Channel *> ChannelList;

        Poller(EventLoop *loop);
        ~Poller();

        /**
         * @brief Polls the I/O events,must be call in the loop thread
         */
        Timestamp poll(int timeoutMs, ChannelList *activeChannel);

        /**
         * @brief Changes the interested I/O events 
         * @param channel 
         */
        void updateChannel(Channel *channel);
        
        // Remove the channel, when it destructs
        void removeChannel(Channel *channel);

        void assertInLoopThread() { ownerLoop_->assertInLoopThread(); }
    private:
        void fillActiveChannel(int numEvents, 
                        ChannelList *avtiveChannels) const;
        typedef std::vector<struct pollfd> PollFdList;
        typedef std::map<int, Channel *> ChannelMap;

        EventLoop *ownerLoop_;
        PollFdList pollfds_;
        ChannelMap channels_;
};
#endif