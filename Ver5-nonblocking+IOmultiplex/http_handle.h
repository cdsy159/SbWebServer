#pragma once
#include"local.h"
#include<memory>
#include"cache.h"
#include"state.h"
#include"local.h"
#define MAXIOBUF 1024
class HttpHandle:public noncopyable
{
public:
    HttpHandle();
    ~HttpHandle()
    {
        file.reset();
    }
    void init(int sockfd)
    {
        fd=sockfd;
        nWrite=0;
        nRead=0;
        nSolve=0;
        keepAlive=false;
        sendFile=false;
    }
    bool readtBuf();
    int processRead();
    int processWrite();
    void serveStatic();
    void getLine(char*);
    void readRequest();
    bool isStatic(char*)    
    void serveStatic();
    void clienterror(const char*,const char*,const char*);
    void addResponse(const char*);
private:
    int fd;
    std::shared_ptr<FileInfo*> file; 
    //write buf 
    char writebuf[MAXIOBUF];
    int nWrite;
    //read buf
    char readbuf[MAXIOBUF];
    int nRead;
    int nSolve;
    bool keepAlive;
    bool sendFile;
};
