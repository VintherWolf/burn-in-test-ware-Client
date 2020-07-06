/*
 * main.cpp
 *
 *  Created on: Jun 04, 2020
 *      Author: Daniel K. Vinther Wolf
 */

#include <type_traits>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>

#include "menus.hpp"
#include "jsonhandler.h"
#include "connection.h"

using namespace std;

int main(int argC, char **argV, char **envP)
{
	pthread_t tid;

	volatile char usr_input;
	extern char sendSchema[256];

	Menus main_menu;

	struct conParams *readParams;

	char ip_addr[16];
	strncpy(ip_addr, main_menu.servIP.c_str(), 16);
	// Default IP and Port (0.0.0.0 1955)
	readParams = (struct conParams *)malloc(sizeof(*readParams));
	readParams->schema = sendSchema;
	readParams->servIP = ip_addr;
	readParams->servPort = 1955;

	while ((usr_input = main_menu.main_menu()) != quit)
	{

		if (usr_input == -1)
		{
			usr_input = 0;
			continue;
		}

		/* Spawn a new thread to handle this connection
		5099 means that the schema is not empty */
		if (valReq(sendSchema) > 5099)
		{
			strncpy(ip_addr, main_menu.servIP.c_str(), 16);
			readParams->servIP = ip_addr;
			readParams->servPort = main_menu.servPort;

			if (pthread_create(&tid, NULL, ConThread, readParams) != 0)
			{
				perror("An error occurred while starting new connection thread.");
			}
		}
		// Just wait a bit. ConThread may write to stdout
		//usleep(100000);
		sleep(1);
	}

	cout << "Quitting!" << endl;
	free(readParams);
	return EXIT_SUCCESS;
}
