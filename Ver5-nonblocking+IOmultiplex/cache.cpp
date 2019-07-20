#include"cache.h"
boost::shared_ptr<FileInfo*> getFile(char* filename)
{
    if(cache.find(filename)!=cache.end())
        return cache[filename];
    else
    {
        if(size>=MAXCACHE)
            cleancache();

    }
}
