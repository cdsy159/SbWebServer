#include <iostream>
#include"local.h"
#include"net.h"
#include"mutex.h"
int listenfd;
MutexLock mutex;
void* handle_thread(void* arg)
{
    Pthread_detach(pthread_self());
    int *tmp=(int*)arg;
    int z=*tmp;
    printf("pthread %d is starting\n",*tmp);
    delete tmp;
    sockaddr cliaddr;
    socklen_t len=sizeof(cliaddr);
    int connfd;
    while(true)
    {
        {
            MutexLockGuard lock(mutex);
            connfd=Accept(listenfd,&cliaddr,&len);
        }
        printf("pthread %d is working\n",z);
        doit(connfd);
        Close(connfd);
    }
}
int main()
{
    listenfd=initList(8080);
   // sockaddr_in cliaddr;
   // socklen_t len=sizeof(cliaddr);
   // memset(&cliaddr,0,sizeof(cliaddr));
    Listen(listenfd); 
    pthread_t tid[10];
    for(int i=0;i<9;i++)
    {
        int* tmp=new int;
        *tmp=i;
        Pthread_create(&tid[i],NULL,handle_thread,tmp);
    }
    for(;;)
        pause();
    return 0;
}

