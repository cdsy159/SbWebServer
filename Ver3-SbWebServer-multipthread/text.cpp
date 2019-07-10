#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<dirent.h>

//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <string.h>
//#include <ctype.h>
//#include <setjmp.h>
/*#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>*/
//#include <errno.h>
/*#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>*/
using namespace std;
extern int errno;
int main()
{
    errno=2;
    //extern int errno;
    printf("%s\n",strerror(errno));
    //opendir("/home/kk");
    //printf("%d",errno);
    // fprinto(stderr,"%s:%s\n",a,strerror(errno));
    
}

