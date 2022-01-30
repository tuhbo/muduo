/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-28 17:40:33
 */
#include "muduo/base/CurrentThread.h"

__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;

void cacheTid() {
    if (t_cachedTid == 0) {
        t_cachedTid = static_cast<pid_t>(syscall(SYS_gettid));
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

const char *tidString() {
    return t_tidString;
}

int tidStringLength() {
    return t_tidStringLength;
}