
/*
Author:Faris Alotaibi

Description: server-side chat client. 
Connect using ./server username [portnum]
*/

#include "connectfuncs.h"


int main(int argc, char *argv[])
{
	errno = 0;
	int serverfd, clientfd;
	struct sockaddr_in servaddr;
	int port = PORTNUM;
	if(argc < 2 || argc>3)
	{
		fprintf(stderr, "Usage is %s username [port] \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char* username = argv[1];
	if(argv[2]) //We've received a port
	{
		char* endptr = NULL;
		port = strtol(argv[2], &endptr, 10);
		if(port == 0 || (argv[2] == endptr))
		{
			fprintf(stderr, "Invalid port number\n");
			exit(EXIT_FAILURE);
		}
	}

	if((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket");
		exit(EXIT_FAILURE);
	}

	//Setup our sockaddr_in for use in bind
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	char* ip = nameToAddress("localhost"); //Server assumed to be running here
	servaddr.sin_port = htons(port);

	if(inet_pton(AF_INET, ip, &servaddr.sin_addr) <=0)
	{
		fprintf(stderr, "inet_pton error\n");
		exit(EXIT_FAILURE);
	}

	if ((bind(serverfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
	{
		perror("Bind");
		exit(EXIT_FAILURE);
	}

	if((listen(serverfd, LISTENQ)) < 0)
	{
		perror("Listen");
		exit(EXIT_FAILURE);
	}

	strcat(username,": ");
	while(1)
	{
		clientfd = accept(serverfd, NULL, NULL);
		if(chat(username, clientfd) == 0) break;//If the server chose to exit
	}
	//Cleanup
	close(serverfd);
	exit(EXIT_SUCCESS);
}
