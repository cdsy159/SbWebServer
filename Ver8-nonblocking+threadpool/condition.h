#ifndef _CONDITION_H_
#include<pthread.h>
#include"mutex.h"
#include"noncopyable.h"
#define _CONDITION_H_
class Condition:noncopyable
{
public:
    Condition(MutexLock& mutex):m_mutex(mutex)
    {
        pthread_cond_init(&m_cond,NULL);
    }
    ~Condition()
    {
        pthread_cond_destroy(&m_cond);
    }
    void wait()
    {
        MutexLock::UnassignGuard tmp(m_mutex);
        pthread_cond_wait(&m_cond,m_mutex.getPthreadMutex());
    }
    void notify()
    {
        pthread_cond_signal(&m_cond);
    }
    void notifyAll()
    {
        pthread_cond_broadcast(&m_cond);
    }
private:
    MutexLock& m_mutex;
    pthread_cond_t m_cond;
};
#endif
