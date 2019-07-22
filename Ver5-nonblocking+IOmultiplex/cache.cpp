#include"cache.h"
boost::shared_ptr<FileInfo*> Cache::getFile(char* filename)
{
    //为了避免多个线程同时请求同一个不存在于cache中的资源，在新建这个资源到cache时要加锁.
    if(cache.find(filename)!=cache.end())
        return cache[filename];
    else
    {
        if(size>=MAXCACHE)
            cleancache();
        
    }
}
