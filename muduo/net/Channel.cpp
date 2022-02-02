/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-02 17:49:37
 */
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Logging.h"
#include <poll.h>


const int Channel::KNoneEvent = 0;
const int Channel::KReadEvent = POLLIN | POLLPRI;
const int Channel::KWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fdArg)
    : loop_(loop),
    fd_(fdArg),
    events_(0),
    revents_(0),
    index_(-1)
{
}

void Channel::update() {
    loop_->updateChannel(this);
}

void Channel::handleEvent() {
    if (revents_ & POLLNVAL) {
        LOG_WARN << "Channel::handle_event() POLLNVAL";
    }

    if (revents_ & (POLLERR | POLLNVAL)) {
        if (errorCallback_) {
            errorCallback_();
        }
    }

    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (readCallback_) {
            readCallback_();
        }
    }

    if (revents_ & POLLOUT) {
        if (writeCallback_) {
            writeCallback_();
        }
    }
}