#pragma once
#include<assert.h>
#include<pthread.h>
#include"noncopyable.h"
class MutexLock:noncopyable 
{
public:
    MutexLock()
        :holder(0)
    {
        pthread_mutex_init(&mutex_,NULL);
    }
    ~MutexLock()
    {
        assert(holder==0);
        pthread_mutex_destroy(&mutex_);
    }
    void lock()
    {
        pthread_mutex_lock(&mutex_);
        resign();
    }
    void unlock()
    {
        unresign();
        pthread_mutex_unlock(&mutex_);
    }
private:
    void resign()
    {
        holder=pthread_self();
    }
    void unresign()
    {
        holder=0;
    }
    pthread_mutex_t mutex_;
    pid_t holder;

};
class MutexLockGuard:noncopyable
{
public:
    MutexLockGuard(MutexLock& mutex):mutex_(mutex)
    {
        mutex_.lock();
    }
    ~MutexLockGuard()
    {
        mutex_.unlock();
    }
private:
    MutexLock& mutex_;
};
