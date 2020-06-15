/*
 * client.cpp
 *
 *  Created on: Jun 04, 2020
 *      Author: Daniel K. Vinther Wolf
 */

#include <type_traits>
#include <iostream>
#include <ctype.h>
#include "client.h"
#include "menus.hpp"
#include "jsonhandler.h"
#include "connection.h"

using namespace std;

int main(int argC, char **argV) {

	char usr_input;
	extern char sendSchema[256];
	Menus main_menu;
	int tcpThread = -1;

	// Spawn a new thread to handle this connection
	pthread_t tid;

	do {
		main_menu.show_menu();
		usr_input = main_menu.user_input();

		if (usr_input == -1)
			continue;


	if ((valReq(sendSchema) > 5099 || usr_input == getRelay) && usr_input != quit) {

		if (pthread_create(&tid, NULL, ConThread, (void*)sendSchema) != 0){
			perror(
					"An error occurred while starting new connection thread.");
		}


	}

} while (usr_input != quit);

cout << "Quitting!" << endl;

return EXIT_SUCCESS;
}

