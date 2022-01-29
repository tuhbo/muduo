/*
 * @Description: 
 * @Version: 1.0
 * @Autor: tuhbo
 * @Date: 2022-01-28 17:40:33
 */
#include "muduo/base/CurrentThread.h"

__thread int t_cachedTid = 0; /*线程局部变量*/

void cacheTid() {
    if (t_cachedTid == 0) {
        t_cachedTid = static_cast<pid_t>(syscall(SYS_gettid));
    }
}