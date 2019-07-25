#include <iostream>
#include"local.h"
#include"net.h"
#include"mutex.h"
#include"cache.h"
#include"epoll_wrap.h"
#include"http_handle.h"
#include"state.h"
#include<sys/epoll.h>
int listenfd;
MutexLock mutex;
Cache g_cache;
int main()
{
    listenfd=initList(8080);
   // sockaddr_in cliaddr;
   // socklen_t len=sizeof(cliaddr);
   // memset(&cliaddr,0,sizeof(cliaddr));
    Listen(listenfd); 
    SetNonBlocking(listenfd);
    struct epoll_event events[100];
    Http_Handle handler[100];
    struct epoll_event ev;
    ev.data.fd=listenfd;
    ev.events=EPOLLIN;
    int epfd=Epoll_create(80);
    Epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    sockaddr_in cliaddr;
    socklen_t clilen;
    while(true)
    {
        int num=Epoll_wait(epfd,events,100,-1);
        for(int i=0;i<num;i++)
        {
            if(events[i].data.fd==listenfd)
            {
               int connfd;
               if((connfd=accept(listenfd,(sockaddr*)&cliaddr,&clilen))<0)
               {
                   if(connfd==EAGAIN)
                   {
                        continue;
                   }
                   else
                       unix_error("accept failured!\n");
               }
               ev.data.fd=connfd;
               ev.events=EPOLLIN;
               Epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
               handler[connfd].init(connfd);
            }
            else if(events[i].events&EPOLLIN)
            {
                int tmp=events[i].data.fd;
                int state;
                if((state=handler[tmp].processRead())==STATE_WRITE)
                {
                    events[i].events=EPOLLOUT;
                    Epoll_ctl(epfd,EPOLL_CTL_MOD,events[i].data.fd,&events[i]);
                }
                else//STATE_ERROR
                {
                    Epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,&events[i]);
                    Close(tmp);
                }
            }
            else if(events[i].events&EPOLLOUT)
            {
                int tmp=events[i].data.fd;
                int state;
                if((state=handler[tmp].processWrite())==STATE_READ)
                {

                }
                else
                {
                    if(state==STATE_SUCCESS&&handler[tmp].isAlive())
                    {
                        events[i].events=EPOLLIN;
                        Epoll_ctl(epfd,EPOLL_CTL_MOD,events[i].data.fd,&events[i]);
                        continue;
                    }
                    Epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,&events[i]);
                    Close(tmp);     
                }
            }
            else
                unix_error("Epoll_wait failured!\n");
        }
    }
     return 0;
}

