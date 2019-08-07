#include"net.h"
const char* root_dir="/home/xf/goodgoodstudy/WebSiteSrc/html_book_20150808/reference";
const char* home_page ="index.html";
void doit(int fd)
{
    Servbuf servbuf;
    char usrbuf[MAXLINE];
    BindServbuf(fd,&servbuf);
    readServbufline(&servbuf,usrbuf);
    struct stat sbuf;
    char method[256],url[256],version[256];
    sscanf(usrbuf,"%s %s %s",method,url,version);
    if(strcasecmp(method,"GET")!=0)
    {
        //TODO:send error to client 
        return ;
    }
    //TODO:solve the request header
    
    char filename[MAXLINE];
    parse_url(url,filename);
    if(stat(filename,&sbuf)<0)
    {
        //TODO:inform the client that the filename doesn't exit 
        return;
    }
    serve_static(fd,filename,sbuf.st_size); 
}
void get_filetype(char* filename,char* filetype)
{
    if(strstr(filename,".html"))
        strcpy(filetype,"text/html");
    else if(strstr(filename,".gif"))
        strcpy(filetype,"image/gif");
    else if(strstr(filename,".jpg"))
        strcpy(filetype,"image/jpg");
    else if(strstr(filename,",png"))
        strcpy(filetype,"image/png");
    else if(strstr(filename,".css"))
        strcpy(filetype,"text/css");
    else if(strstr(filename,".ttf")||strstr(filename,".otf"))
        strcpy(filetype,"application/octet-stream");
    else
        strcpy(filetype,"text/plain");
}
void serve_static(int clientfd,char* filename,int filesize)
{
    char buf[MAXLINE];
    char filetype[MAXLINE];
    int filefd;
    sprintf(buf,"HTTP/1.0 200 OK\r\n");
    get_filetype(filename,filetype);
    sprintf(buf,"%sServer: Sb Web Server\r\n",buf);
    sprintf(buf,"%sContent-length: %d\r\n",buf,filesize);
    sprintf(buf,"%sContent-type:%s\r\n\r\n",buf,filetype);
    Writen(clientfd,buf,strlen(buf));
    char* pbuff;
    filefd=Open(filename,O_RDONLY,0);
    pbuff=(char*)Mmap(0,filesize,PROT_READ,MAP_PRIVATE,filefd,0);
    Close(filefd);
    Writen(clientfd,pbuff,filesize);
    Munmap(pbuff,filesize);
}
void parse_url(char* url,char* filename)
{
    char* ptrack=filename;
    sscanf(root_dir,"%s",filename);
    ptrack=ptrack+strlen(root_dir);
    
    if(strlen(url)==1)
    {
        if(*url=='/')
        {
            sprintf(ptrack,"/%s",home_page);
           // sscanf(home_page,"%s",ptrack);
            return;
        }
       else
       {
           //TODO:can it happened?
       }
    }
    sscanf(url,"%s",ptrack);
    return;
}

