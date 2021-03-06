# include(CheckFunctionExists)

# check_function_exists(accept4 HAVE_ACCEPT4)
# if(NOT HAVE_ACCEPT4)
#   set_source_files_properties(SocketsOps.cc PROPERTIES COMPILE_FLAGS "-DNO_ACCEPT4")
# endif()

set(net_SRCS
  EventLoop.cpp
  Channel.cpp
  Poller.cpp
  Timer.cpp
  TimerQueue.cpp
  EventLoopThread.cpp
  InetAddress.cpp
  SocketsOps.cpp
  Socket.cpp
  Acceptor.cpp
  TcpServer.cpp
  TcpConnection.cpp
  Buffer.cpp
  EventLoopThreadPool.cpp
  Connector.cpp
  TcpClient.cpp
)

add_library(muduo_net ${net_SRCS})
target_link_libraries(muduo_net muduo_base)


install(TARGETS muduo_net DESTINATION lib)

set(HEADERS
  EventLoop.h
  Channel.h
  Poller.h
  Callbacks.h
  Timer.h
  TimerId.h
  TimerQueue.h
  EventLoopThread.h
  InetAddress.h
  Socket.h
  SocketOps.h
  Acceptor.h
  TcpServer.h
  TcpConnection.h
  Buffer.h
  EventLoopThreadPool.h
  Connector.h
  TcpClient.h
  )
install(FILES ${HEADERS} DESTINATION include/muduo/net)



if(MUDUO_BUILD_EXAMPLES)
  add_subdirectory(tests)
endif()

