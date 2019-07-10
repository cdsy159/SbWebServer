edit:Server.o net.o local.o
	g++ -o edit Server.o net.o local.o
Server.o:Server.cpp 
	g++ -c -g  Server.cpp
net.o:net.cpp 
	g++ -c -g net.cpp
local.o:local.cpp
	g++ -c -g  local.cpp
clean:
	rm *.o edit
