#include"thread_pool.h"
ThreadPool::ThreadPool(int threadNum,int maxQuequeSize)
    :threadNum_(threadNum),maxQueueSize_(maxQueueSize)
     ,mutex_(),notEmpty_(mutex_),notFull_(mutex_)
{
    assert(threadNum>=1&&maxQueueSize>=1);
    for(int i=0;i<threadNum;i++)
    {
        pthread_t tid_t;
        Pthread_create(&tid_t,NULL,startThread,this);
    }
}
size_t ThreadPool::queueSize()
{
    MutexLockGuard lock(mutex_);
    return queue_.size();
}
ThreadPool::~ThreadPool()
{

}
void* ThreadPool::startThread(void* obj)
{
    Pthread_detach(Pthread_self());
    printf("Woking thread is %u",Pthread_self());
    pool->run();
    return pool;
}
ThreadPool::Task ThreadPool::take()
{
    MutexLockGuard lock(mutex_);
    while(queue_.empty())
        notEmpty_.wait();
    Task task;

}
