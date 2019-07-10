#pragma once
#ifndef _local_h_
#define _local_h_
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/stat.h>
#define LISTENQ 50
#define MAXBUFFER 8192
#define MAXLINE 1024
extern int errno;
typedef struct 
{
    int fd;
    int cnt;
    char* ptr;
    char buff[MAXBUFFER];
}Servbuf;
void unix_error(const char* msg);
int Socket();
void Bind(int fd,sockaddr* servaddr);
int initList(int port); 
void Listen(int fd);
int Accept(int fd,sockaddr* clientaddr,socklen_t* len);
void BindServbuf(int fd,Servbuf* servbuf);
int readServbuf(Servbuf* servbuf,void* usrbuf,size_t n);
int readServbufline(Servbuf* servbuf,void* usrbuf);
void Munmap(void*,size_t);
void Close(int);
void* Mmap(void*,size_t,int,int,int,off_t);
int Open(char*,int,mode_t);
ssize_t Writen(int,void*,size_t);
ssize_t writen(int,void*,size_t);
#endif

