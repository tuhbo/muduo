/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-28 18:09:08
 */
#ifndef BASE_LOGSTREAM_H
#define BASE_LOGSTREAM_H

#include <muduo/base/noncopyable.h>
#include <assert.h>
#include <string.h>
#include <string>

using std::string;
const int KSmallBuffer = 4000;
const int KLargeBuffer = 4000 * 1000;

template <int SIZE>
class FixedBuffer : noncopyable {
    public:
        FixedBuffer()
            : cur_(data_)
        {
            setCookie(cookieStart);
        }
        
        ~FixedBuffer() {
            setCookie(cookieEnd);
        }

        void append(const char *buf, size_t len) {
            if (static_cast<size_t>(avail()) > len) {
                memcpy(cur_, buf, len);
                cur_ += len;
            }
        }

        const char *data() const { return data_; }

        int length() const { return static_cast<int>(cur_ - data_); }

        char *current() { return cur_; }

        int avail() const { return static_cast<int>(end() - cur_); }

        void add(size_t len) { cur_ += len; }

        void reset() { cur_ = data_; }

        void bzero() { memset(data_, 0, sizeof(data_)); }

        const char *debugString();

        void setCookie(void (*cookie)()) { cookie_ = cookie; }

        string toString() const { return string(data_, length()); }
        
    private:
        const char *end() const { return data_ + sizeof(data_); }

        static void cookieStart();
        static void cookieEnd();

        void (*cookie_)();
        char data_[SIZE];
        char *cur_;
};

class LogStream : noncopyable {
    public:
        typedef FixedBuffer<KSmallBuffer> Buffer;

        LogStream &operator<<(bool v) {
            buffer_.append(v ? "1" : "0", 1);
            return *this;
        }

        LogStream &operator<<(short);
        LogStream &operator<<(unsigned short);
        LogStream &operator<<(int);
        LogStream &operator<<(unsigned int);
        LogStream &operator<<(long);
        LogStream &operator<<(unsigned long);
        LogStream &operator<<(long long);
        LogStream &operator<<(unsigned long long);

        LogStream &operator<<(const void *);

        LogStream &operator<<(float v) {
            *this << static_cast<double>(v);
            return *this;
        }

        LogStream &operator<<(double);

        LogStream &operator<<(char v) {
            buffer_.append(&v, 1);
            return *this;
        }

        LogStream &operator<<(const char *str) {
            if (str) {
                buffer_.append(str, strlen(str));
            } else {
                buffer_.append("(null)", 6);
            }
            return *this;
        }

        LogStream &operator<<(const unsigned char *str) {
            return operator<<(reinterpret_cast<const char *>(str));
        }

        LogStream &operator<<(const string &v) {
            buffer_.append(v.c_str(), v.size());
            return *this;
        }

        LogStream &operator<<(Buffer &v) {
            *this << v.toString();
            return *this;
        }

        void append(const char *data, int len) { buffer_.append(data, len); }

        const Buffer& buffer() const { return buffer_; }

        void resetBuffer() { buffer_.reset(); }
        
    private:
        
        template <typename T>
        void formatInteger(T);
        
        Buffer buffer_;

        static const int KMaxNumericSize;
};

class Fmt {
    public:
        template<typename T>
        Fmt(const char* fmt, T val);

        const char* data() const { return buf_; }
        int length() const { return length_; }

    private:
        char buf_[32];
        int length_;
};

inline LogStream& operator<<(LogStream& s, const Fmt& fmt) {
    s.append(fmt.data(), fmt.length());
    return s;
}

#endif