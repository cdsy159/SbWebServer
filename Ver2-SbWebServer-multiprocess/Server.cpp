#include <iostream>
#include"local.h"
#include"net.h"

void addsig(int sign,void (*handler)(int),bool restart=true)
{
    struct sigaction solve;
    solve.sa_handler=handler;
    if(restart)
    {
        solve.sa_flags|=SA_RESTART;
    }
    sigfillset(&solve.sa_mask);
    int rc;
    if((rc=sigaction(sign,&solve,NULL))<0)
        unix_error("sinal set failure!");
    return;
}
void Eat(int sign )
{
    pid_t rc;
    while(rc=waitpid(-1,NULL,WNOHANG)>0);
    return;
}
int main()
{
    int listenfd=initList(8080);
    int confd;
    sockaddr_in cliaddr;
    socklen_t len=sizeof(cliaddr);
    memset(&cliaddr,0,sizeof(cliaddr));
    Listen(listenfd); 
    while(true)
    {
        addsig(SIGCHLD,Eat);
        confd=Accept(listenfd,(struct sockaddr*)&cliaddr,&len);
        pid_t pid;
        if((pid=Fork())==0)//child
        {
            Close(listenfd);
            doit(confd);  
            printf("the process %d is running\n",getpid());
            exit(0);
        }
        Close(confd);
    }
    return 0;
}

