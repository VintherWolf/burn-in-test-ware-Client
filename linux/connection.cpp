/*
 * connection.c
 *
 *  Created on: Mar 5, 2012
 *      Author: klausk
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <syslog.h>
#include <iostream>

#include "jsonhandler.h"


const char EndOfFile = 0x04; 				// Ctrl + D
const char DONT_UNDERSTAND[] = "Sorry I don't understand that command\n";

#define SERVER_IP "192.168.6.2"
#define SERVER_PORT 1955
using namespace std;
void *ConThread (void *p)
{
	// TCP IP Setup:
	puts("Creating Connection thread");
	int sockfd = -1;
	struct sockaddr_in servAddr;
	char servIP[] = SERVER_IP;
	unsigned short servPort = SERVER_PORT;
	//struct addrinfo addrInfo;

	/*
	memset(&addrInfo, 0, sizeof(addrInfo));
	addrInfo.ai_family = PF_INET;
	addrInfo.ai_socktype = SOCK_STREAM;
	addrInfo.ai_protocol = IPPROTO_TCP;
	 */

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIP);
	servAddr.sin_port = htons(servPort);

	// Buffers
	const int buflen = 256;
	char recBuffer[buflen];
	//char wrtBuffer[buflen];

	// Test Variables
	//int n;
	//int req = 0;
	int mesgLen = 0;

	if((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		puts("Socket failed!");
		close (sockfd);
		exit(EXIT_FAILURE);
	}


	if(connect(sockfd, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0){
		puts("Connect failed!");
		close (sockfd);
		return NULL;
	}

	mesgLen = strlen((char*)p);
	if( send(sockfd, (char*)p, mesgLen,0) != mesgLen){
		puts("Send failed!");
		close (sockfd);
		return NULL;
	}

	if( recv(sockfd, recBuffer, 256,0) <= 0){
		puts("Receive failed!");
		close (sockfd);
		return NULL;
	}
	cout << "Received: " << recBuffer << endl;
	//printf("Received from server %s\n", recBuffer);
	// Clean up before leaving the room

	close (sockfd);

	return NULL;
}
