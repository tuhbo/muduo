set(base_SRCS
  CurrentThread.cpp
  Date.cpp
  FileUtil.cpp
  LogFile.cpp
  Logging.cpp
  LogStream.cpp
  Timestamp.cpp
  Thread.cpp
  ThreadPool.cpp
  TimeZone.cpp
  )

add_library(muduo_base ${base_SRCS})
target_link_libraries(muduo_base pthread rt)

install(TARGETS muduo_base DESTINATION lib)


file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/muduo/base)

if(MUDUO_BUILD_EXAMPLES)
  add_subdirectory(test)
endif()
