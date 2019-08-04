edit:Server.o net.o local.o epoll_wrap.o cache.o http_handle.o thread_pool.o
	g++ -o edit Server.o net.o local.o epoll_wrap.o cache.o http_handle.o thread_pool.o -lpthread 
Server.o:Server.cpp 
	g++ -c -g  Server.cpp
net.o:net.cpp 
	g++ -c -g net.cpp
local.o:local.cpp
	g++ -c -g  local.cpp
epoll_wrap.o:epoll_wrap.cpp
	g++ -c -g epoll_wrap.cpp
cache.o:cache.cpp
	g++ -c -g cache.cpp
http_handle.o:http_handle.cpp
	g++ -c -g http_handle.cpp
thread_pool.o:thread_pool.cpp
	g++ -c -g thread_pool.cpp
clean:
	rm *.o edit
