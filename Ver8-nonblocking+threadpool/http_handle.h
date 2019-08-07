#pragma once
#ifndef _HTTP_HANDLE_H_
#define _HTTP_HANDLE_H_ 
#include"local.h"
#include<memory>
#include"cache.h"
#include"state.h"
#include<stdarg.h>
#include"epoll_wrap.h"
#define MAXIOBUF 1024
class Http_Handle:public noncopyable
{
public:
    Http_Handle(){}
    ~Http_Handle()
    {
        //file.reset();
    }
    enum State{Read,Write};
    void init(int sockfd);
    void process();
    bool readtBuf();
    int processRead();
    int processWrite();
    void serveStatic();
    void getLine(char*);
    void readRequest();
    bool isStatic(char*);    
    void serveStatic(char*,char*);
    void getFiletype(char*,char*);
    void clienterror(const char*,const char*,const char*,const char* );
    void addResponse(char*);
    void addResponse(const char*,...);
    bool isAlive();
    void parseurl(char*,char*);
    void clear();
    void setState(State s);
    static void setEpollfd(int epollfd);
private:
    int fd;
    static int m_epollfd;
    std::shared_ptr<FileInfo> file; 
    //write buf 
    char writebuf[MAXIOBUF];
    int nWrite;
    int nSend;
    //read buf
    char readbuf[MAXIOBUF];
    int nRead;
    int nSolve;
    bool keepAlive;
    bool sendFile;
    State m_state;
};
#endif
