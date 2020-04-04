/*
Author:Faris Alotaibi

Description: Headers needed by both client 
and server.
*/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>      
#include <stdio.h>        
#include <unistd.h>      
#include <sys/select.h>  
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

#define MAXLINE 4096
#define BUFFSIZE 8192
#define LISTENQ 1024
#define PORTNUM 13000
#define TIMEOUT_SECS 120

int chat(char* username, int chatteefd);
char* nameToAddress(char* addrname);
