#ifndef _CACHE_H_
#define _CACHE_H_ 
#include"local.h"
#include<memory>
#include<map>
#define MAXCACHE 100
class FileInfo:public noncopyable 
{
public:
    FileInfo(const char* filename) ;
    ~FileInfo();
private:
    void* addr;
    //int size;
    int count;
};
class Cache:public noncopyable
{
public:
    boost::shared_ptr<FileInfo*>  getFile(char*);
private:
    void cleancache();
private:
    unordered_map<string,boost::shared_ptr<FileInfo*> > cache;
    //unordered_map<string,int> dict;
    int size;
    MutexLock mutex_; 
};
#endif 
