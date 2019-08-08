#include <iostream>
#include"local.h"
#include"net.h"
#include"mutex.h"
#include"cache.h"
#include"epoll_wrap.h"
#include"http_handle.h"
#include"state.h"
#include<sys/epoll.h>
#include"condition.h"
#include"thread_pool.h"
int listenfd;
MutexLock mutex;
Cache g_cache;
int Http_Handle::m_epollfd=0;
int main()
{
    Sigaction(SIGPIPE,SIG_IGN,false);
    listenfd=initList(8080);
   // sockaddr_in cliaddr;
   // socklen_t len=sizeof(cliaddr);
   // memset(&cliaddr,0,sizeof(cliaddr));
    Listen(listenfd); 
    //SetNonBlocking(listenfd);
    struct epoll_event events[100];
    //Http_Handle handler[100];
    struct epoll_event ev;
    ev.data.fd=listenfd;
    ev.events=EPOLLIN|EPOLLET;
    int epfd=Epoll_create(80);
    Http_Handle::setEpollfd(epfd);
    Http_Handle handler[100];
    Epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    ThreadPool pool(2,300); 
    sockaddr_in cliaddr;
    socklen_t clilen;
    while(true)
    {
        int num=Epoll_wait(epfd,events,100,-1);
        for(int i=0;i<num;i++)
        {
            int tmpfd=events[i].data.fd;
            if(events[i].data.fd==listenfd)
            {
                int connfd;
                for(;;)
                {
                    if((connfd=accept(listenfd,(sockaddr*)&cliaddr,&clilen))<0)
                    {
                        if(errno==EAGAIN)
                        {
                            break;
                        }
                        else
                            unix_error("accept failured!\n");
                    }
                    addfd(epfd,EPOLLIN,connfd);
                    handler[connfd].init(connfd);   
                }
            }
            else
                pool.append(boost::bind(&Http_Handle::process,&handler[events[i].data.fd]));
        }
    }       
    unix_error("Server is shutdown now!");
    return 0;
}

