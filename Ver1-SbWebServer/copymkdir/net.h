#pragma once
#include"local.h"
const char* root_dir="/home/xf/goodgoodstudy/webserver/01.simpleSer/page/Spweb/WebSiteSrc/html_book_20150808/reference";
const char* home_page="index.html";
void doit(int);
void serve_static(int,char*,int);
void parse_url(char*,char*);

