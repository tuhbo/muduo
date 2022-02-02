/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-02 16:05:35
 */
#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "muduo/base/CurrentThread.h"
#include "muduo/base/noncopyable.h"

#include <vector>
#include <memory>

class Channel;
class Poller;

class EventLoop : noncopyable {
    public:
        EventLoop();
        ~EventLoop();

        void quit();

        void loop();

        void updateChannel(Channel* channel);

        void assertInLoopThread() {
            if (!isInLoopThread()) {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const {
            return threadId_ == ::tid();
        }
    private:
        void abortNotInLoopThread();

        typedef std::vector<Channel *> ChannelList;

        bool looping_;
        bool quit_;
        const pid_t threadId_;
        std::unique_ptr<Poller> poller_;
        ChannelList activeChannels_;

};

#endif