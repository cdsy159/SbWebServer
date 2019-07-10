edit:web_regular.o http_server.o csapp.o
	g++ -o edit web_regular.o http_server.o csapp.o
web_regular.o:web_regular.cpp http_server.h csapp.h
	g++ -c -g  web_regular.cpp
http_server.o:http_server.cpp http_server.h csapp.h
	g++ -c -g http_server.cpp
csapp.o:csapp.cpp csapp.h
	g++ -c -g  csapp.cpp
clean:
	rm *.o edit
