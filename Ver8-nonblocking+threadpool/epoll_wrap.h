#pragma once
#include"local.h"
#include<sys/epoll.h>
int Epoll_create(int);
int Epoll_ctl(int,int,int,struct epoll_event*);
int Epoll_wait(int,struct epoll_event*,int,int);
void addfd(int,int,int);
void modfd(int,int,int);
void removefd(int,int);
