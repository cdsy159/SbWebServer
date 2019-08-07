#pragma once
#include"local.h"
#include<sys/epoll.h>
int Epoll_create(int);
int Epoll_ctl(int,int,int,struct epoll_event*);
int Epoll_wait(int,struct epoll_event*,int,int);
void addfd(int,int,struct epoll_event*);
void modfd(int,int,struct epoll_event*);
void removefd(int,int);
