edit:Server.o net.o local.o epoll_wrap.o cache.o http_handle.o 
	g++ -o edit Server.o net.o local.o epoll_wrap.o cache.o http_handle.o -lpthread 
Server.o:Server.cpp 
	g++ -c -g  Server.cpp
net.o:net.cpp 
	g++ -c -g net.cpp
local.o:local.cpp
	g++ -c -g  local.cpp
epoll_wrap.p:epoll_wrap.cpp
	g++ -c -g epoll_wrap.cpp
cache.o:cache.cpp
	g++ -c -g cache.cpp
http_handle.o:http_handle.cpp
	g++ -c -g http_handle.cpp
clean:
	rm *.o edit
