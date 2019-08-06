#include <iostream>
#include"epoll_wrap.h"
int Epoll_create(int size)
{
    int rc;
    rc=epoll_create(size);
    if(rc<0)
        unix_error("epoll failured!\n");
    return rc;
}
int Epoll_ctl(int epfd,int op,int fd,struct epoll_event* event)
{
    int rc;
    if((rc=epoll_ctl(epfd,op,fd,event))<0)
    {
        /*if(op==EPOLL_CTL_ADD)
            printf("add wrong\n");
        else if(op==EPOLL_CTL_MOD)
            printf("mod wrong\n");
        else if(op==EPOLL_CTL_DEL)
            printf("del wrong\n");*/
        unix_error("epoll_ctl failured!\n");
    }
    if(op==EPOLL_CTL_ADD)
        SetNonBlocking(fd);
    return rc;
}
int Epoll_wait(int epfd,struct epoll_event* events,int maxevents,int timeout)
{
    return epoll_wait(epfd,events,maxevents,timeout);
}
void addfd(int epollfd,int fd,bool one_shot)
{
    epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN|EPOLLET;
    if(one_shot)
        event.events|=EPOLLONESHOT;
    Epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
}
void modfd(int epollfd,int fd,int ev)
{
    epoll_event event;
    event.data.fd=fd;
    event.events=ev|EPOLLET;
    Epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}
void removefd(int epollfd,int fd)
{
    Epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    //printf("the thread %u start close!\n",(unsigned int)(pthread_self()));
    Close(fd);
    //printf("the thread %u have closed!\n",(unsigned int)(pthread_self()));
}
