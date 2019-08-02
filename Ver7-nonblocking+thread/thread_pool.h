#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_ 
#include<list>
#include<boost/function.hpp>
#include<boost/bind.hpp>
#include"mutex.h"
#include"condition.h"
#include"local.h"
#include"noncopyable.h"
#include"epoll_ulti.h"
class ThreadPool:noncopyable
{
public:
    typedef boost::function<void()> Task;
    ThreadPool(int threadNum,int maxTaskNum);
    ~ThreadPool();
    bool append(Task&&);
    void run();
    static void* startThread(void* arg);
private:
    bool isFull();
    Task take();
    size_t queueSize();
    int threadNUm_;
    int maxQuequeSize_;
    std::list<Task> queue_;
    MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
};
#endif
