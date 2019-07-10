#include <iostream>
#include"local.h"
#include"net.h"
void* handle(void *arg)
{
    int *tmp=(int*)arg;
    int fd=*tmp;
    delete tmp;
    printf("thread %d is working:solve fd %d\n",pthread_self(),fd);
    doit(fd);
    printf("thread %d is working: close fd %d\n",pthread_self(),fd);
    Close(fd);
}
int main()
{
    int listenfd=initList(8080);
    sockaddr_in cliaddr;
    socklen_t len=sizeof(cliaddr);
    memset(&cliaddr,0,sizeof(cliaddr));
    Listen(listenfd); 
    while(true)
    {
        int *connfd=new int;
        *connfd=Accept(listenfd,(struct sockaddr*)&cliaddr,&len);
        pthread_t tid;
        Pthread_create(&tid,NULL,handle,connfd);
    }
    return 0;
}

