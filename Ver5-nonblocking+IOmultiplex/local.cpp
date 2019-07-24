#include"local.h"
void unix_error(const char* msg)
{
    fprintf(stderr,"%s: %s",msg,strerror(errno));
    exit(0);
}
int Socket()
{
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd<0)
        unix_error("socket failure!");
    return fd;
}
void Bind(int fd,sockaddr* servaddr)
{
    if(bind(fd,servaddr,sizeof(sockaddr))<0)
        unix_error("bind failure!");
}
int initList(int port)
{
    int listenfd=Socket();
    int optival=1;
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optival,sizeof(int))<0)
        unix_error("REUSEADDR failured!");
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    unsigned short tmpport=8080;
    servaddr.sin_port=htons(tmpport);
    Bind(listenfd,(sockaddr*)&servaddr);
    return listenfd;
}
void Listen(int fd)
{
    if(listen(fd,LISTENQ)<0)
        unix_error("Listen failure");
}
int Accept(int listenfd,sockaddr* client,socklen_t* len)
{
    int rc;
    if((rc=accept(listenfd,client,len))<0)
        unix_error("accept failure!");
    return rc;
}
void SetNonBlocking(int fd)
{
    int flags=fcntl(fd,F_GETFL,0);
    flags|=O_NONBLOCK;
    fcntl(fd,F_SETFL,flags);
}
void BindServbuf(int fd,Servbuf* servbuf)
{
    servbuf->fd=fd;
    servbuf->cnt=0;
    servbuf->ptr=servbuf->buff;
}
int readServbuf(Servbuf* servbuf,void* usrbuf,size_t n)
{
    char* buf=(char*)usrbuf;
    int rc;
    while(servbuf->cnt<=0)
    {
        if((rc=read(servbuf->fd,servbuf->buff,sizeof(servbuf->buff)))<0)
            unix_error("fill the Servbuf failure!");
        else if(rc==0)
            return 0;//EOF
        else
        {
            servbuf->cnt=rc;
            servbuf->ptr=servbuf->buff;
        }
    }
    int num=(servbuf->cnt>n)?n:servbuf->cnt;
    memcpy(buf,servbuf->ptr,num);
    servbuf->cnt=servbuf->cnt-num;
    servbuf->ptr=servbuf->ptr+num;
    return num;
}
int readServbufline(Servbuf* servbuf,void* usrbuf)
{
    char* buf=(char*)usrbuf;
    int rc;
    char* ptrack=buf;
    int n=0;
    char tmp;
    while(true)
    {
        if((rc=readServbuf(servbuf,ptrack,1))==1)
        {
            n++;
            tmp=*ptrack;
            ptrack++;
            if(tmp=='\n')
                break;
        }
        else if(rc==0)
            return n;
        else
            unix_error("readServbufline failure!");
       // printf("i'm coming\n");
    }
    return n;
}
void Munmap(void* addr,size_t len)
{
    int rc;
    if((rc=munmap(addr,len))<0)
        unix_error("Munmap failured");
    return ;
}
void Close(int fd)
{
    int rc;
    if((rc=close(fd))<0)
        unix_error("close failured");
    return;
}
void* Mmap(void* addr,size_t len,int prot,int flag,int fd,off_t off)
{
    void* ptr;
    if((ptr=mmap(addr,len,prot,flag,fd,off))==MAP_FAILED)
        unix_error("map failured!");
    return ptr;
}
int Open(char* pathname,int flags,mode_t mode)
{
    int rc;
    if((rc=open(pathname,flags,mode))<0)
        unix_error("open failured!");
    return rc;
}
ssize_t Writen(int fd,void* usrbuf,size_t n)
{
    int rc;
    if((rc=writen(fd,usrbuf,n))<0)
        unix_error("writen failured!");
    return rc;
}
ssize_t writen(int fd,void* usrbuf,size_t n)
{
    int nleft=n;
    char* buff=(char*)usrbuf;
    int rc;
    char* ptrack=buff;
    while(nleft>0)
    {
        if((rc=write(fd,ptrack,nleft))<=0)
        {
            if(errno==EINTR)
                continue;
            else
                return -1;
        }
        else
        {
            nleft-=rc;
            ptrack+=rc;
        }
    }
    return n;
}
void Pthread_create(pthread_t *tidp,const pthread_attr_t* attr,void* (*handle)(void*),void* arg)
{
    int rc;
    if((rc=pthread_create(tidp,attr,handle,arg))!=0)
        unix_error("Pthread create failured!");
}
void Pthread_detach(pthread_t tid)
{
    int rc;
    if((rc=pthread_detach(tid))<0)
        unix_error("pthread detach failured!");
}
