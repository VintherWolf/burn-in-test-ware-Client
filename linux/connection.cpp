/*
 * connection.c
 *
 *  Created on: Mar 5, 2012
 *      Author: klausk
 *      Edited by: Daniel K. Vinther Wolf (June-2020)
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
#include "connection.h"

const char EndOfFile = 0x04; // Ctrl + D
const char DONT_UNDERSTAND[] = "Sorry I don't understand that command\n";

using namespace std;
void *ConThread(void *args)
{
	struct conParams *params = (struct conParams*)args;

	// TCP IP Setup:
	puts("Creating Connection thread");
	int sockfd = -1;
	struct sockaddr_in servAddr;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(params->servIP);
	servAddr.sin_port = htons(params->servPort);

	// Buffers
	const int buflen = 256;
	char recBuffer[buflen];

	int mesgLen = 0;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		puts("Socket failed!");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
	{
		puts("Connect failed!");
		close(sockfd);
		return NULL;
	}

	mesgLen = strlen(params->schema);
	if (send(sockfd, params->schema, mesgLen, 0) != mesgLen)
	{
		puts("Send failed!");
		close(sockfd);
		return NULL;
	}

	if (recv(sockfd, recBuffer, 256, 0) <= 0)
	{
		puts("Receive failed!");
		close(sockfd);
		return NULL;
	}
	cout << "Received: " << recBuffer << endl;

	close(sockfd);

	return NULL;
}
