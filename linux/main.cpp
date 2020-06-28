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

#include "client.h"
#include "menus.hpp"
#include "jsonhandler.h"
#include "connection.h"

using namespace std;
	/*
Test test;

test.a = 0;
test.b = 1;
*/

int main(int argC, char **argV)
{
	pthread_t tid;

	char usr_input;
	extern char sendSchema[256];
		Menus main_menu;


	main_menu.show_menu();
	while ((usr_input = main_menu.user_input()) != quit)
	{

		if (usr_input == -1)
			continue;

		/* Spawn a new thread to handle this connection */
		if ((valReq(sendSchema) > 5099 || usr_input == getRelay) && usr_input != quit)
		{

			if (pthread_create(&tid, NULL, ConThread, (void *)sendSchema) != 0)
			{
				perror("An error occurred while starting new connection thread.");
			}
		}
		// Just wait a bit. ConThread may write to stdout
		usleep(10000);
		main_menu.show_menu();
	}

	cout << "Quitting!" << endl;

	return EXIT_SUCCESS;
}
