/*
 * tcpServer.c
 *
 *  Created on: Jun 15 2020
 *      Author: Daniel K. Vinther Wolf
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <arpa/inet.h>

#include "connection.h"

void *Th_TCP_RPC(void *ptr)
{
	int sockfd = -1;
	int newsockfd = -1;
	int retval;
	unsigned int clntscktLen;

	struct sockaddr_in servAddr;

	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == -1)
	{
		syslog(LOG_NOTICE, "Error opening socket");
		exit(EXIT_FAILURE);
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(1955);

	retval = bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr));
	if (retval == -1)
	{
		close(sockfd);
		syslog(LOG_NOTICE, "Error binding");
	}

	retval = listen(sockfd, 10);
	if (retval == -1)
	{
		perror("listen failed");
		return NULL;
	}

	puts("Server listening for connections");

	// Wait for a call to arrive
	while (1)
	{
		struct sockaddr_in clntsckt;
		clntscktLen = sizeof(clntsckt);

		// When a call comes in spawn a new thread and hand over the socket
		newsockfd = accept(sockfd, (struct sockaddr *)&clntsckt, &clntscktLen);

		if (newsockfd < 0)
		{
			syslog(LOG_NOTICE, "Got a wrong socket %d", newsockfd);
			syslog(LOG_NOTICE, "%d= %s", errno, strerror(errno));
			continue;
		}

		syslog(LOG_NOTICE, "Connection with %s\n", inet_ntoa(clntsckt.sin_addr));
		printf("Connection with % s\n", inet_ntoa(clntsckt.sin_addr));
		// Spawn a new thread to handle this connection
		pthread_t tid;
		if (pthread_create(&tid, NULL, ConThread, &newsockfd) != 0)
		{
			perror("An error occurred while starting new connection thread.");
		}
	}
	return NULL;
}
