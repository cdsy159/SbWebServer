#ifndef _CACHE_H_
#define _CACHE_H_ 
#include"local.h"
#include<memory>
#include<map>
#include"mutex.h"
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<iostream>
#define MAXCACHE 100
class FileInfo:public noncopyable 
{
public:
    FileInfo(const char* filename,bool& flag) ;
    ~FileInfo();
   // bool cmp(FileInfo*);
    void Add();
    int Size();
    int Count();
    void* Addr();
private:
    void* addr;
    int size;
    int count;
};
class Cache:public noncopyable
{
public:
    std::shared_ptr<FileInfo>  getFile(char*);
private:
    void cleancache();
private:
    std::unordered_map<std::string,std::shared_ptr<FileInfo> > cache;
    std::vector<std::shared_ptr<FileInfo> > dict; 
    //int size;
    MutexLock mutex_; 
};
#endif 
