#ifndef _MUTEX_H_
#define _MUTEX_H_ 
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
    pthread_mutex_t* getPthreadMutex()
    {
        return &mutex_;
    }
private:
    friend class Condition;
    class UnassignGuard:noncopyable
    {
    public:
        UnassignGuard(MutexLock& mutex):m_mutex(mutex)
        {
            m_mutex.unresign();    
        }
        ~UnassignGuard()
        {
            m_mutex.resign();
        }
    private:
        MutexLock& m_mutex;
    };
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
#endif
