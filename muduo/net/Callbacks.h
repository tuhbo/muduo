/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-02-03 11:01:41
 */
#ifndef NET_CALLBACKS_H
#define NET_CALLBACKS_H

#include "muduo/base/Timestamp.h"
#include <functional>
#include <memory>

class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void()> TimerCallback;

typedef std::function<void (const TcpConnectionPtr &)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr &,
                            Buffer *buf,
                            Timestamp )> MessageCallback;

typedef std::function<void (const TcpConnectionPtr &)> CloseCallback;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
#endif