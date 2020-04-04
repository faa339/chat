FLAGS = -Wall -O1 -std=c99 

all: server client 

server: server.c connectfuncs.c
	gcc server.c connectfuncs.c ${FLAGS} -o server

client: client.c connectfuncs.c
	gcc client.c connectfuncs.c ${FLAGS} -o client

clean: 
	rm -f server client
	