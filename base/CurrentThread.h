/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-28 17:27:01
 */
#ifndef BASE_CURRENTTHREAD_H
#define BASE_CURRENTTHREAD_H


#include <unistd.h>
#include <sys/syscall.h>

extern __thread int t_cachedTid;

void cacheTid();

inline int tid() {
    if (__builtin_expect(t_cachedTid == 0, 0)) {
        cacheTid();
    }
    return t_cachedTid;
}
#endif