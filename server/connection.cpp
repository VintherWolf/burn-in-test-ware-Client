/*
 * connection.c
 *
 *  Created on: Mar 5, 2012
 *      Author: klausk
 *      Edited by Daniel K. Vinther Wolf (jun-2020)
 *      - implemented JSON-RPC
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <syslog.h>
#include <signal.h>

#include "jsonhandler.h"

using namespace std;

#define RESP_SCHEMA "/usr/bbb-projekter/response.json"

const char GETTEMP[] = "GET TEMP";
const int lenGetTemp = strlen (GETTEMP);
const char EndOfFile = 0x04; 				// Ctrl + D
const char DONT_UNDERSTAND[] = "Sorry I don't understand that command, or ID is greater than 10\n";

// From the readtemp module
extern float curTemp;

void *ConThread (void *p)
{
	int sockfd;
	const int buflen = 128;
	char recBuffer[buflen];
	char wrtBuffer[buflen];
	int n;

	char *respSchema;
	respSchema = (char*) malloc(256 * sizeof(char));
	loadSchema(RESP_SCHEMA, respSchema);

	int req = 0;
	int id = -1;

	// Grab a copy of the socket fd
	sockfd = *(int *) p;

	syslog (LOG_NOTICE,
	        "ConThread started conversation with client on socketfd: %d.",
	        sockfd);

	memset (recBuffer, 0, buflen);

	// Continuously read from the socket and echo it until the socket dies out
	n = recv (sockfd, recBuffer, buflen, 0);
	if (n < 0)
	{
		syslog (LOG_NOTICE, "Failed to recv. n = %d", n);
		return NULL;
	}
	while (n > 0)
	{
		syslog (LOG_NOTICE, "Got this from client: \"%s\" length: %d.", recBuffer, n);

		// Special Case GET TEMP request
		if (strncmp (recBuffer, GETTEMP, lenGetTemp) == 0)
		{
			// Echo back the temperature
			memset (wrtBuffer, 0, buflen);
			sprintf (wrtBuffer, "%f\n", curTemp);
			if (write (sockfd, wrtBuffer, strlen (wrtBuffer)) < 0)
			{
				syslog (LOG_NOTICE, "Failed to write to port");
				return NULL;
			}
		}

		else if (recBuffer[0] == EndOfFile)
		{
			n = 0;
			syslog (LOG_NOTICE, "EndOfFile received. Goodbye!");
			continue;
		}

		/* If we got a JSON Formatted string i.e. JSON-RPC */
		else if (valReq(recBuffer) > 5000)
		{
			req = handleReq(recBuffer);
			id = getID(recBuffer);
			req = handleResp(respSchema, req, id);

			// Echo back the response schema if no errors
			/* TODO Implement error handling in accordance with RPC 2.0 */
			memset (wrtBuffer, 0, buflen);
			sprintf (wrtBuffer, "%s", respSchema);
			syslog(LOG_NOTICE, "Sending to client: %s\n", respSchema);
			if (write (sockfd, wrtBuffer, strlen(wrtBuffer)) < 0)
			{
				syslog (LOG_NOTICE, "Failed to write to port");
				return NULL;
			}
		}

		else
		{
			if (write (sockfd, DONT_UNDERSTAND, strlen (DONT_UNDERSTAND)) < 0)
			{
				syslog (LOG_NOTICE, "Failed to write DONT_UNDERSTAND to port");
				return NULL;
			}
		}
		memset (recBuffer, 0, buflen);
		// Read again
		n = recv (sockfd, recBuffer, buflen, 0);
	}
	// Clean up before leaving the room

	close (sockfd);
	free(respSchema);
	syslog (LOG_NOTICE,
	        "Exited the Communications thread. Still listening for new connections.");
	return NULL;
}
