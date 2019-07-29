#pragma once
#include"local.h"
#include<memory>
#include"cache.h"
#include"state.h"
#include<stdarg.h>
#define MAXIOBUF 1024
class Http_Handle:public noncopyable
{
public:
    Http_Handle(){}
    ~Http_Handle()
    {
        //file.reset();
    }
    void init(int sockfd)
    {
        fd=sockfd;
        nWrite=0;
        nSend=0;
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
    bool isStatic(char*);    
    void serveStatic(char*,char*);
    void getFiletype(char*,char*);
    void clienterror(const char*,const char*,const char*,const char* );
    void addResponse(char*);
    void addResponse(const char*,...);
    bool isAlive();
    void parseurl(char*,char*);
    void clear();
private:
    int fd;
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
