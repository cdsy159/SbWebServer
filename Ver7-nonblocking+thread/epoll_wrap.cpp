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
        unix_error("epoll_ctl failured!\n");
    SetNonBlocking(fd);
    return rc;
}
int Epoll_wait(int epfd,struct epoll_event* events,int maxevents,int timeout)
{
    return epoll_wait(epfd,events,maxevents,timeout);
}
