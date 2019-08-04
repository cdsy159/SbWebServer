#include"thread_pool.h"

ThreadPool::ThreadPool(int threadNum,int maxQueueSize)
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
    Pthread_detach(pthread_self());
    //printf("Woking thread is %u\n",pthread_self());
    ThreadPool* pool=static_cast<ThreadPool*>(obj);
    pool->run();
    return pool;
}
void ThreadPool::run()
{
    for(;;)
    {
        Task task(take());
        if(task)
        {
            task();
        }
    }
}
ThreadPool::Task ThreadPool::take()
{
    MutexLockGuard lock(mutex_);
    while(queue_.empty())
        notEmpty_.wait();
    Task task;
    if(!queue_.empty())
    {
        task=queue_.front();
        queue_.pop_front();
        if(maxQueueSize_>0)
            notFull_.notify();
    }
    return task;
}
bool ThreadPool::isFull()
{
    return maxQueueSize_>0&&queue_.size()>=maxQueueSize_;
}
bool ThreadPool::append(Task&& task)
{
    {
        MutexLockGuard lock(mutex_);
        while(isFull())
            notFull_.wait();
        queue_.push_back(std::move(task));
    }
    notEmpty_.notify();
}
