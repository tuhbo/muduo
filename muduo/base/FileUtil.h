/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-29 17:56:04
 */
#ifndef BASE_FILEUTIL_H
#define BASE_FILEUTIL_H

#include "muduo/base/noncopyable.h"
#include <string>
#include <sys/types.h> // for off_t

using std::string;

class ReadSmallFile : noncopyable {
    public:
        ReadSmallFile(string filename);
        ~ReadSmallFile();

        template <typename T>
        int readToString(int maxSize,
                            T *content,
                            int64_t *fileSize,
                            int64_t *modifyTime,
                            int64_t *createTime);
        
        int readToBuffer(int *size);

        const char *buffer() const { return buf_; }


        static const int KBufferSize = 64 * 1024;
    private:
        int fd_;
        int err_;
        char buf_[KBufferSize];
};

template <typename T>
int readFile(string filename, 
            int maxSize,
            T *content, 
            int64_t *fileSize = NULL,
            int64_t *modifyTime = NULL,
            int64_t *createTime = NULL)
{
    ReadSmallFile file(filename);
    return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

class AppendFile : noncopyable {
    public:
        explicit AppendFile(string filename);

        ~AppendFile();

        void append(const char *logline, size_t len);

        void flush();

        off_t writtenBytes() const { return writtenBytes_; }
        
    private:
        size_t write(const char *logline, size_t len);

        FILE *fp_;
        char buffer_[64 * 1024];
        off_t writtenBytes_;
};

#endif