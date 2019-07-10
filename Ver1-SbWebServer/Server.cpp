#include <iostream>
#include"local.h"
#include"net.h"

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
        confd=Accept(listenfd,(struct sockaddr*)&cliaddr,&len);
        doit(confd);   
        Close(confd);
    }
    return 0;
}

