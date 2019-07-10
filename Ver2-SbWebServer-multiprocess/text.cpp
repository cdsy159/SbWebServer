#include <iostream>
#include<unistd.h>
#include<errno.h>
#include<stdio.h>
#include<string.h>
int main()
{
    int fd=10;
    close(fd);
    printf("wrong:%s\n",strerror(errno));
    return 0;
}

