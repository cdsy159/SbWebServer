#include <iostream>
#include"http_handle.h"
extern const char* home_page;
extern const char* root_dir;
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
        memcpy(buf,readbuf+nSolve,1);
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
    char filetype[MAXLINE];
    if(readtBuf()==false)
        return STATE_ERROR;
    getLine(buf);
    sscanf(buf,"%s %s %s",method,url,version);
    if(strcasecmp(method,"GET"))
    {
        clienterror("Can Only support GET","501","Can Only support GET","Use GET plz!");
        return STATE_WRITE;
    }//TODO:clienterror()....
    parseurl(url,filename);
    readRequest();
    //isStatic();
    serveStatic(filename,filetype);
    return STATE_WRITE;
}
void HttpHandle::serveStatic(char* filename,char* filetype)
{
    //写入响应行和响应头
    // sprintf(writebuf+nWrite,"HTTP/1.0 200 OK\r\n");//响应行
    //addResponse("HTTP/1.0 200 OK\r\n");
    file=g_cache.getFile(filename);
    if(file==NULL)
    {
        clienterror("No Resource","404","No Resource","Use other Page!");//所寻找的资源不存在
        return;
    }
    addResponse("HTTP/1.0 200 OK\r\n");
    getFiletype(filename,filetype);
    //struct stat fileinfo;
    if(file->Size()<0)
        unix_error("Get FileInfo failured!\n");
    addResponse("Content-length: %d\r\n",file->Size());
    addResponse("Content-type: %s\r\n\r\n",filetype);
    sendFile=true;
}
void HttpHandle::getFiletype(char* filename,char* filetype)
{
    if(strstr(filename,".html"))
        strcpy(filetype,"text/html");
    else if(strstr(filename,".gif"))
        strcpy(filetype,"image/gif");
    else if(strstr(filename,"image/jpeg"))
        strcpy(filetype,"image/jpeg");
    else if(strstr(filename,".png"))
        strcpy(filetype,"image/png");
    else if(strstr(filename,".css"))
        strcpy(filetype,"text/css");
    else if(strstr(filename,".ttf")||strstr(filename,".otf"))
        strcpy(filetype,"application/octet-stream");
    else
        strcpy(filetype,"text/plain");

}
void HttpHandle::addResponse(char* body)
{
        int num=strlen(body);
        if(nWrite+strlen(body)>MAXIOBUF)
            return;//TODO:所写内容超过缓冲区大小
        sprintf(writebuf+nWrite,"%s",body);
        nWrite+=num;
}
void HttpHandle::addResponse(const char* format,...)
{
        va_list args;
        va_start(args,format);
        int ret=vsprintf(writebuf+nWrite,format,args);
        va_end(args);
        if(ret>=0)
            nWrite+=ret;
        return;
}
void HttpHandle::clienterror(const char* cause,const char* errnum,const char* shortmsg,const char* longmsg)
{
        //2019 7.20 TODO:完成clienterror
        char body[MAXLINE];
        sprintf(body,"<html><title>SbWebError</title>");
        sprintf(body,"%s<body bgcolor=""ffffff"">\r\n",body);
        sprintf(body,"%s%s: %s\r\n",body,errnum,shortmsg);
        sprintf(body,"%s<p>%s: %s\r\n",body,longmsg,cause);
        sprintf(body,"%s<hr><em>The SbWebServer</em>\r\n",body);

        addResponse("HTTP/1.0 %s %s\r\n",errnum,shortmsg);
        addResponse("Content-type: text/html\r\n");
        addResponse("Content-length: %d\r\n\r\n",(int)strlen(body));
        addResponse(body);
        return;

}
void HttpHandle::parseurl(char* url,char* filename)
{
    sprintf(filename,"%s",root_dir);
    sprintf(filename+strlen(filename),"%s",url);
    if(url[strlen(url)-1]=='/')
        sprintf(filename+strlen(filename),"%s",home_page);
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
int Http_Handle::processWrite()
{
    int rc;
    while(nWrite<strlen(writeBuf))
    {
        if((rc==write(fd,writeBuf+nWrite,strlen(writeBuf)-nWrite))<0)
        {
            if(rc==EAGAIN)
                return STATE_WRITE;
            else
                retrun STATE_ERROR;
        }
        nWrite+=rc;
    }
    if(sendFile)
    {
        int pos=nWrite-strlen(nWrite);
        while(true)
        {
            if((rc=write(fd,(char*)(file->addr)+pos,file->size-pos))<0)
            {
                if(rc==EAGAIN)
                    return STATE_WRITE;
                else
                    return STATE_ERROR;
            }
            else if(rc==0)
                break;
            else
                nWrite+=rc;
        }
    }
    if(keepAlive)
        return STATE_READ;
    return STATE_SUCCESS;
}
bool isAlive()
{
    return keepAlive; 
}
