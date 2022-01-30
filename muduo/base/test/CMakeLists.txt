add_executable(fileutil_test FileUtil_test.cc)
target_link_libraries(fileutil_test muduo_base)
add_test(NAME fileutil_test COMMAND fileutil_test)


add_executable(logfile_test LogFile_test.cc)
target_link_libraries(logfile_test muduo_base)

add_executable(logging_test Logging_test.cc)
target_link_libraries(logging_test muduo_base)


# if(BOOSTTEST_LIBRARY)
# add_executable(logstream_test LogStream_test.cc)
# target_link_libraries(logstream_test muduo_base boost_unit_test_framework)
# add_test(NAME logstream_test COMMAND logstream_test)
# endif()

add_executable(thread_test Thread_test.cc)
target_link_libraries(thread_test muduo_base)

add_executable(threadpool_test ThreadPool_test.cc)
target_link_libraries(threadpool_test muduo_base)
