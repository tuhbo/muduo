/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-29 22:02:38
 */
#ifndef BASE_LOGFILE_H
#define BASE_LOGFILE_H

#include <memory>
#include <string>
#include <mutex>
#include "muduo/base/noncopyable.h"

using std::string;

class AppendFile;

class LogFile : noncopyable {
    public:
        LogFile(const string& basename,
            off_t rollSize,
            bool threadSafe = true,
            int flushInterval = 3,
            int checkEveryN = 1024);
        ~LogFile();

        void append(const char* logline, int len);
        void flush();
        bool rollFile();
    private:
        void append_unlocked(const char *logline, int len);
        static string getLogFileName(const string& basename, time_t* now);

        const string basename_; //日志文件的basename
        const off_t rollSize_; // 日志文件的大小达到rollSize_就换一个新文件
        const int flushInterval_; //日志写入间隔时间
        const int checkEveryN_; //当count >= checkEveryN_时，检查一下滚动的条件

        int count_;

        std::unique_ptr<std::mutex> mutex_;
        time_t startOfPeriod_; //开始记录日志时间
        time_t lastRoll_; //上一次滚动日志文件时间
        time_t lastFlush_; // 上一次日志写入文件时间
        std::unique_ptr<AppendFile> file_;

        const static int kRollPerSeconds_ = 60*60*24;
};
#endif