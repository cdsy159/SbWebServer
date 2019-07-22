#include"cache.h"
boost::shared_ptr<FileInfo*> Cache::getFile(char* filename)
{
    //为了避免多个线程同时请求同一个不存在于cache中的资源，在新建这个资源到cache时要加锁.
    MutexLockGuard lock(mutex_);
    if(cache.find(filename)!=cache.end())
    {
        cache[filename]->count+=1;
        return cache[filename];
    }
    else
    {
        if(size>=MAXCACHE)
            cleancache();
        {
            bool flag;
            //MutexLockGuard lock(mutex_);
            FileInfo* ptr=new FileInfo(filename,flag);
            if(!flag)
                return NULL;
            cache[filename]=make_shared<FileInfo>(*ptr);
           // dict.push_back(ptr);
            return cache[filename];
        }
    }
}
FileInfo::FileInfo(const char* filename,bool& flag)
{
    int fd=open(filename,O_RDONLY);
    if(fd<0)
    {
        flag=false;
        return;
    }
    struct stat fileinfo;
    stat(filename,&fileinfo);
    size=fileinfo.st_size;
    addr=Mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
    count=1;
    flag=true;
}
FileInfo::~FileInfo()
{
    Munmap(addr,size);
    return;
}
void Cache::cleancache()
{
    
}
/*
 1.cleancache里面不用加锁，因为调用cleancache时已经加锁了，若在加锁则形成死锁。
 2.cleancache内对heap和unordered_map进行erase,erase并不会影响到正在往某个socket中写文件内容的线程，因为该线程持有一个shared_ptr,即使erase掉了heap与unordered_map中的元素
 由于引用计数至少还为1，FileInfo还不会析构，直到引用计数为0.
 3.vector每次只有到满了后才重新排序，即在cleancache()内进行排序，排序完后再erase.
 4.排序使用qsort，需要自定义一个compare仿函数，同时对FileInfo的<进行重载
 */ 
