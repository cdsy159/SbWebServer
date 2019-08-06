#include <iostream>
#include"local.h"
#include"net.h"
#include"mutex.h"
#include"cache.h"
#include"epoll_wrap.h"
#include"http_handle.h"
#include"state.h"
#include<sys/epoll.h>
#include"thread_pool.h"
int listenfd;
MutexLock mutex;
Cache g_cache;
int Http_Handle::epollfd=0;
void block_sigpipe()
{
    sigset_t signal_mask;
    sigemptyset(&signal_mask);
    sigaddset(&signal_mask,SIGPIPE);
    int rc=pthread_sigmask(SIG_BLOCK,&signal_mask,NULL);
    if(rc!=0)
        printf("block sigpipe error\n");
}
int main()
{
    //Sigaction(SIGPIPE,SIG_IGN,false);
    listenfd=initList(8080);
   // sockaddr_in cliaddr;
   // socklen_t len=sizeof(cliaddr);
   // memset(&cliaddr,0,sizeof(cliaddr));
    Listen(listenfd); 
    //SetNonBlocking(listenfd);
    block_sigpipe();
    struct epoll_event events[100];
    //Http_Handle handler[100];
    struct epoll_event ev;
    ev.data.fd=listenfd;
    ev.events=EPOLLIN|EPOLLET;
    int epfd=Epoll_create(80);
    Http_Handle::setepollfd(epfd); 
    Http_Handle handler[100];
    ThreadPool pools(2,300);
    Epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    //Epoll_ctl(epfd,EPOLL_CTL_DEL,100,&ev);
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
                        //printf("now the accept idx:%d solve the fd:%d\n",i,connfd);
                        if(errno==EAGAIN)
                        {
                            break;
                        }
                        else
                            unix_error("accept failured!\n");
                    }
                    ev.data.fd=connfd;
                    ev.events=EPOLLIN|EPOLLET|EPOLLONESHOT;
                    Epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
                    handler[connfd].init(connfd);   
                }
            }
            else
            {
                //printf("now the idx:%d solve the fd:%d\n",i,tmpfd);
                pools.append(boost::bind(&Http_Handle::process,&handler[tmpfd]));
            }  
        }
    }
    unix_error("Server is shutdown now!");
    return 0;
}

