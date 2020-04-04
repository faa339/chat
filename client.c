
/*
Author:Faris Alotaibi

Description: client-side chat client. 
Connect using ./client username [ip address] [portnum]
*/

#include "connectfuncs.h"


int main(int argc, char* argv[])
{
	errno = 0;
	int clientfd;
	struct sockaddr_in servaddr;
	int port = PORTNUM;
	char* ip;
	if(argc < 2)
	{
		fprintf(stderr, "Usage is %s username [ip address][port]\n"
				, argv[0]);
		exit(EXIT_FAILURE);
	}

	if(argc==3) //If we've been given an ip address or hostname
		ip = nameToAddress(argv[2]);
	else
		ip = nameToAddress("localhost");

	if (ip == NULL) //Invalid ip/hostname given 
	{
		fprintf(stderr, "Invalid ip address or hostname: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	if(argc==4)
	{
		char* endptr = NULL;
		port = strtol(argv[3], &endptr, 10);
		if(port == 0)
		{
			fprintf(stderr, "Invalid port number: %d\n", port);
			exit(EXIT_FAILURE);
		}
	}


	char* username = argv[1];

	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	if(inet_pton(AF_INET, ip, &servaddr.sin_addr) <=0)
	{
		if(errno)
			perror("inet_pton");
		else
			fprintf(stderr, "%s is not in AF_INET\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	if((connect(clientfd, (struct sockaddr *)&servaddr, 
				sizeof(servaddr))) < 0)
	{
		perror("Connect error");
		exit(EXIT_FAILURE);
	}
	//Format username
	strcat(username,": ");
	chat(username, clientfd);
	exit(EXIT_SUCCESS);
}
