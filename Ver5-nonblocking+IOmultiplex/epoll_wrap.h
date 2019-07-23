#pragma once
#include"loca.h"
#include<sys/epoll.h>
int Epoll_create(int);
int Epoll_ctl(int,int,int,struct epoll_event*);
int Epoll_wait(int,struct epoll_event*,int,int);
