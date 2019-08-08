#ifndef _THREAD_POOL_H_
#include<pthread.h>
#include"noncopyable.h"
#include<boost/function.hpp>
#include<boost/bind.hpp>
#include<queue>
#include"mutex.h"
#include"condition.h"
#define _THREAD_POOL_H_
class ThreadPool:noncopyable
{
public:
typedef boost::function<void()> Task;
    ThreadPool(int threadNum,int maxSize)
        :m_threadNum(threadNum),m_maxSize(maxSize),m_mutex(),m_notFull(m_mutex),m_notEmpty(m_mutex)
    {
        assert(m_threadNum>0&&m_maxSize>0);
        for(int i=0;i<threadNum;i++)
        {
            pthread_t tmpThread;
            pthread_create(&tmpThread,NULL,startThread,this);
        }
    }
    ~ThreadPool()
    {

    }
    static void* startThread(void*);
    void append(Task&&);
    bool isFull();
private:
    Task Take();
    void Work();
private:
    std::queue<Task> m_queue;
    int m_threadNum;
    int m_maxSize;
    MutexLock m_mutex;
    Condition m_notFull;
    Condition m_notEmpty;
};
#endif
