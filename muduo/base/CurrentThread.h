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
#include <stdio.h>

extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;

void cacheTid();

inline int tid() {
    if (__builtin_expect(t_cachedTid == 0, 0)) {
        cacheTid();
    }
    return t_cachedTid;
}

const char *tidString();

int tidStringLength();
#endif