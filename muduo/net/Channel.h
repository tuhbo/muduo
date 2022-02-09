/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-02 11:23:03
 */
#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include "muduo/base/noncopyable.h"
#include "muduo/base/Timestamp.h"
#include <functional>
class EventLoop;

/**
 * @brief A selectable I/O channel
 * 
 * this calss doesn't own the file descriptor
 * 
 */
class Channel : noncopyable {
    public:
        typedef std::function<void()> EventCallback;
        typedef std::function<void(Timestamp)> ReadEventCallback;
        Channel(EventLoop *loop, int fd);

        ~Channel();

        void handleEvent(Timestamp receiveTime);

        void setReadCallback(const ReadEventCallback &cb) {
            readCallback_ = std::move(cb);
        }

        void setWriteCallback(const EventCallback &cb) {
            writeCallback_ = std::move(cb);
        }

        void setErrorCallback(const EventCallback &cb) {
            errorCallback_ = std::move(cb);
        }

        void setCloseCallback(const EventCallback &cb) {
            closeCallback_ = std::move(cb);
        }

        int fd() const { return fd_; }

        int events() const { return events_; }

        void set_revents(int revt) { revents_ = revt; }

        bool isNoneEvent() const { return events_ == KNoneEvent; }

        void enableReading() { events_ |= KReadEvent; update(); }

        void enableWriting() { events_ |= KWriteEvent; update(); }

        void disableWriting() { events_ &= ~KWriteEvent; update(); }

        void disableAll() { events_ = KNoneEvent; update(); }
        
        bool isWriting() const { return events_ & KWriteEvent; }
        int index() { return index_; }

        void set_index(int idx) { index_ = idx; }

        EventLoop *ownerLoop() { return loop_; }
    private:
        void update();

        static const int KNoneEvent;
        static const int KReadEvent;
        static const int KWriteEvent;

        EventLoop *loop_;
        const int fd_;
        int events_;
        int revents_;
        int index_;

        bool eventHandling_;

        ReadEventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback errorCallback_;
        EventCallback closeCallback_;
};
#endif