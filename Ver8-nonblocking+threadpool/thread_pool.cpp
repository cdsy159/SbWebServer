#include"thread_pool.h"
void* ThreadPool::startThread(void* arg)
{
    pthread_detach(pthread_self());
    ThreadPool* pool=(ThreadPool*)arg;
    pool->Work();    
    return arg;
}
void ThreadPool::Work()
{
    for(;;)
    {
        Task task=Take();
        if(task)
        {
            task();
        }
    }   
}
ThreadPool::Task ThreadPool::Take()
{
    MutexLockGuard lock(m_mutex);
    while(m_queue.empty())
        m_notEmpty.wait();
    Task task;
    if(!m_queue.empty())
    {
        task=m_queue.front();
        m_queue.pop();
        if(m_maxSize>0)
            m_notFull.notify();
    }
    return task;
}
void ThreadPool::append(Task&& task)
{
    {
        MutexLockGuard lock(m_mutex);
        while(isFull())
        {
            m_notFull.wait();
        }
        m_queue.push(std::move(task));
    }
    m_notEmpty.notify();
}
bool ThreadPool::isFull()//isFull在append中被锁所保护，不需要加锁，但如果在其他地方单独调用时，记得加锁
{
    return (m_maxSize>0)&&(m_queue.size()>=m_maxSize);
}
