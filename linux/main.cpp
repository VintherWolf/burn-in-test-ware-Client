/*
 * client.cpp
 *
 *  Created on: Jun 04, 2020
 *      Author: Daniel K. Vinther Wolf
 */

#include <type_traits>
#include <iostream>
#include <ctype.h>
#include <unistd.h>


#include "menus.hpp"
#include "jsonhandler.h"
#include "connection.h"

using namespace std;


int main(int argC, char **argV)
{
	pthread_t tid;

	volatile char usr_input;
	extern char sendSchema[256];

	struct conParams * readParams;

	readParams = (struct conParams*)malloc(sizeof(*readParams));
	//readParams->schema = sendSchema;
	readParams->schema = sendSchema;
	readParams->servIP = "192.168.0.27";
	readParams->servPort = 1955;


	Menus main_menu;

	main_menu.show_menu();

	while ((usr_input = main_menu.user_input()) != quit)
	{

		if (usr_input == -1)
			continue;

		/* Spawn a new thread to handle this connection */
		if ((valReq(sendSchema) > 5099 || usr_input == getRelay) && usr_input != quit)
		{

			if (pthread_create(&tid, NULL, ConThread, readParams) != 0)
			{
				perror("An error occurred while starting new connection thread.");
			}
		}
		// Just wait a bit. ConThread may write to stdout
		usleep(100000);
		main_menu.show_menu();
	}

	cout << "Quitting!" << endl;
	free(readParams);
	return EXIT_SUCCESS;
}
