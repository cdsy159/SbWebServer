#include <iostream>
#include"http_handle.h"
bool HttpHandle::readtBuf()
{
    int rc;
    while(true)
    {
        if((rc=recv(fd,readbuf,MAXIOBUF-nRead-1,0))<0)
        {
            if(rc==EAGAIN)
                break;
            else
                return false; 
        }
        else
            nRead+=rc;
    }
    return true;
}
void HttpHandle::getLine(char* buf)
{
    int num=0;
    int rc;
    while(true)
    {
        memcpy(buf,readBuf+nSolve,1);
        nSolve++;
        if(*(buf+(num++))=='\n')
            break;
    }
    buf[num]=0;
    return;
}
int HttpHandle::processRead()
{
    char buf[MAXLINE];
    char method[MAXLINE];
    char url[MAXLINE];
    char version[MAXLINE];
    if(readtBuf()==false)
        return STATE_ERROR;
    getLine(buf);
    sscanf(buf,"%s %s %s",method,url,version);
    if(!strcasecmp(method,"GET"))
    {
        
    }
    else
        return STATE_ERROR;
}
void HttpHandl::readRequest()
{
    char buf[MAXLINE];
    while(true)
    {
        getLine(buf);
        if(strstr(buf,"Keep-Alive"))
            keepAlive=true;
        if(strcmp(buf,"\r\n"))
            break;
    }
    return;
}
