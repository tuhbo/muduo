/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-29 21:37:41
 */
#include "muduo/base/FileUtil.h"
#include "muduo/base/Logging.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

AppendFile::AppendFile(string filename)
    : fp_(::fopen(filename.c_str(), "ae")), // 'e' for O_CLOEXRC
    writtenBytes_(0)
{
    assert(fp_);
    ::setbuffer(fp_, buffer_, sizeof buffer_);
}

AppendFile::~AppendFile() {
    ::fclose(fp_);
}

void AppendFile::append(const char *logline, size_t len) {
    size_t written = 0;

    while (written != len) {
        size_t remain = len - written;
        size_t n = write(logline + written, remain);
        if (n != remain) {
            int err = ferror(fp_);
            if (err) {
                fprintf(stderr, "AppendFile::append() failed %s\n", strerror_tl(err));
                break;
            }
        }
        written += n;
    }
    writtenBytes_ += written;
}

void AppendFile::flush() {
    ::fflush(fp_);
}

size_t AppendFile::write(const char *logline, size_t len) {
    return ::fwrite_unlocked(logline, 1, len, fp_);
}

ReadSmallFile::ReadSmallFile(string filename)
    : fd_(::open(filename.c_str(), O_RDONLY | O_CLOEXEC)),
    err_(0)
{
    buf_[0] = '\0';
    if (fd_ < 0) {
        err_ = errno;
    }
}

ReadSmallFile::~ReadSmallFile() {
    if (fd_ >= 0) {
        ::close(fd_);
    }
}

template <typename T>
int ReadSmallFile::readToString(int maxSize,
                            T *content,
                            int64_t *fileSize,
                            int64_t *modifyTime,
                            int64_t *createTime)
{
    assert(content != NULL);
    int err = err_;
    if (fd_ >= 0) {
        content->clear();
        
        if (fileSize) {
            struct stat statbuf;
            if (::fstat(fd_, &statbuf) == 0) {
                if (S_ISREG(statbuf.st_mode)) {
                    *fileSize = statbuf.st_size;
                    content->reserve(static_cast<int>(std::min((int64_t)maxSize, *fileSize)));
                } else if (S_ISDIR(statbuf.st_mode)) {
                    err = EISDIR;
                }
                
                if (modifyTime) {
                    *modifyTime = statbuf.st_mtime;
                }

                if (createTime) {
                    *createTime = statbuf.st_ctime;
                }
            } else {
                err = errno;
            }
        }
        while (content->size() < static_cast<size_t>(maxSize)) {
            size_t toRead = std::min(static_cast<size_t>(maxSize) - content->size(), sizeof(buf_));
            ssize_t n = ::read(fd_, buf_, toRead);
            if (n > 0) {
                content->append(buf_, n);
            } else {
                if (n < 0) {
                    err = errno;
                }
                break;
            }    
        }
    }
    return err;
}

int ReadSmallFile::readToBuffer(int* size) {
    int err = err_;
    if (fd_ >= 0) {
        ssize_t n = ::pread(fd_, buf_, sizeof(buf_)-1, 0);
        if (n >= 0) {
            if (size) {
                *size = static_cast<int>(n);
            }
            buf_[n] = '\0';
        } else {
            err = errno;
        }
    }
    return err;
}

template int readFile(string filename,
                    int maxSize,
                    string* content,
                    int64_t*, int64_t*, int64_t*);

template int ReadSmallFile::readToString(int maxSize, string* content, int64_t*, int64_t*, int64_t*);