#pragma once
#include"local.h"
extern const char* root_dir;
extern const char* home_page;
void doit(int);
void serve_static(int,char*,int);
void get_filetype(char*,char*);
void parse_url(char*,char*);

