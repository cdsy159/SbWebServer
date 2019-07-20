#include <iostream>
#include"http_handle.h"
extern const char* home_page;
extern const char* index;
extern Cache g_cache;
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
    //解析请求，同时处理请求
    //processRead职能：
    //解析头，讲需要写入socket的内容先写入HttpHandle的缓冲区
    //最后从缓冲区写入socket的任务交由processWrite()来进行
    //另外，如果出现非GET,或资源不存在的问题，processRead将需要发送给客户端的错误信息写入HttpHandle的缓冲区
    char buf[MAXLINE];
    char method[MAXLINE];
    char url[MAXLINE];
    char version[MAXLINE];
    char filename[MAXLINE];
    if(readtBuf()==false)
        return STATE_ERROR;
    getLine(buf);
    sscanf(buf,"%s %s %s",method,url,version);
    if(strcasecmp(method,"GET"))
        //TODO:clienterror()....
    parseurl(url,filename);
    readRequest();
    //isStatic();
    serveStatic();
}
void HttpHandle::serveStatic(char* filename)
{
    //写入响应行和响应头
    // sprintf(writebuf+nWrite,"HTTP/1.0 200 OK\r\n");//响应行
    addResponse("HTTP/1.0 200 OK\r\n");
    file=g_cache.getFile(filename);
    if(file==NULL)
        clienterror("No Resource","404","Try Other Page!");//所寻找的资源不存在
}
void HttpHandle::addResponse(const char* body)
{
        int num=strlen(body);
        if(nWrite+body>MAXIOBUF)
            return;//TODO:所写内容超过缓冲区大小
        sprintf(writebuf+nWrite,"%s",body);
        nWrite+=num;
}
void HttpHandle::clienterror(const char* cause,const char* errnum,const char* shortmsg,const char* longmsg)
{
        //2019 7.20 TODO:完成clienterror
        char body[MAXLINE];
        sprintf(body,"<html><title>SbWebError</title>");
        sprintf(body,"%s<body bgcolor")
}
void HttpHandle::parseurl(char* url,char* filename)
{
    sprintf(filename,"%s",home_page);
    sprintf(filename+strlen(filename),"%s",url)
    if(url[strlen(url)-1]=='/')
        sprintf(filename+strlen(filename),"%s",index);
}
void HttpHandle::readRequest()
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
