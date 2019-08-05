#pragma once
#include"local.h"
#include<memory>
#include"cache.h"
#include"state.h"
#include<stdarg.h>
#include"epoll_wrap.h"
#include<unistd.h>
#define MAXIOBUF 1024
class Http_Handle:public noncopyable
{
public:
    Http_Handle(){}
    ~Http_Handle()
    {
        //file.reset();
    }
    static void setepollfd(int);
    void init(int sockfd);
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
    void process();
public:
    static int epollfd;
private:
    enum States{kRead,kWrite,kOver,kError};
    void setState(States s){state_=s;}
private:
    //enum States{kRead,kWrite,kOver,kError};
    //static int epollfd;
    int fd;
    States state_;
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
};
