/*
Author:Faris Alotaibi

Description: Functions used by both the server
and client sides of the chat.
*/
#include "connectfuncs.h"

int chat(char* username, int chatteefd)
{
	//This code deals with nonblocking IO from stdin and 
	//the client being talked to through use of select
	int maxfd, selectnum;
	char inputbuf[MAXLINE];
	char clientbuf[MAXLINE];
	struct timeval timeout;
	fd_set set;

	timeout.tv_sec = TIMEOUT_SECS;
	timeout.tv_usec = 0;
	while(1) //Keep looking for input
	{
		FD_ZERO(&set);
		memset(inputbuf, 0, MAXLINE-1);
		memset(clientbuf, 0, MAXLINE-1);

		maxfd = chatteefd + 1;
		FD_SET(chatteefd, &set);
		FD_SET(0, &set);
		selectnum = select(maxfd, &set, NULL, NULL, &timeout);
		if (selectnum < 0)
		{
			perror("Select failed");
			break;
		}

		if(selectnum == 0)
		{
			fprintf(stderr, "Timed out\n");
			break;
		}

		if(FD_ISSET(0, &set)) //Check if anything received via stdin
		{
			int n = read(0,inputbuf, MAXLINE);
			inputbuf[n] = 0;

			if(strcmp(inputbuf, "exit\n") == 0) //Chat's over
			{
				close(chatteefd);
				return 0;
			}
			//Write the username first, then the message 
			if(write(chatteefd, username, strlen(username) + 1) < 0)
			{
				perror("Self");
				fflush(stdout);
				break;
			}
			if (write(chatteefd, inputbuf, n) < 0)
			{
				perror("Client");
				fflush(stdout);
				break;
			}
		}

		//Check if anything from the chat partner
		if(FD_ISSET(chatteefd, &set))
		{
			int n=read(chatteefd, clientbuf, MAXLINE);
			if(n==0)break; //In case our partner exited
			clientbuf[n] = 0;
			if(write(1, clientbuf, n) < 0)
			{
				perror("Client");
				fflush(stdout);
				break;
			}
		}
	}
	//Leaving it on the caller to close the fd's they passed in here
	return 1;
}


char* nameToAddress(char* addrname)
{
	//Convert given name to presentable address to be fed into inet_pton
	struct addrinfo* results;
	struct addrinfo socktype;
	char* ipname = (char*) malloc(MAXLINE);

	memset(&socktype, 0, sizeof(struct addrinfo));
	socktype.ai_socktype = SOCK_STREAM;
	socktype.ai_family = AF_INET;

	if((getaddrinfo(addrname, NULL, &socktype, &results)) != 0)
		return NULL;

	//Take the first result -- we only need 1 address
	struct sockaddr_in* addr = (struct sockaddr_in *) results->ai_addr;
	inet_ntop(AF_INET, &addr->sin_addr.s_addr, ipname, MAXLINE);
	//Return the presentable addr name -- can pass that to 
	//an inet_pton call 
	return ipname;
}