/*
 * menus.cpp
 *
 *  Created on: Jun 04, 2020
 *      Author: Daniel K. Vinther Wolf
 */

#include <iostream>
#include <string>
#include "menus.hpp"
#include <new>
#include "jsonhandler.h"
#include "connection.h"

// RapidJSON
#include "../include/rjson/reader.h"
#define REQ_SCHEMA "./request.json"

char sendSchema[256];
/* MENUS */

using namespace rapidjson;
using namespace std;

Menus::Menus()
{
	this->input = 0;
	this->input_method = 0;

	for (int i = 0; i < MENU_LINES; i++)
	{
		this->menu[i] = {" "};
	}

	this->menu[0] = {"Select:"};
	this->menu[1] = {"1 Load Schema"};
	this->menu[2] = {"2 Methods"};
	this->menu[3] = {" "};
	this->menu[4] = {" "};
	this->menu[5] = {" "};
	this->menu[6] = {"q quit"};
}

Menus::~Menus()
{
	delete[] menu;
}

void Menus::show_menu(void)
{
	puts("");
	for (int i = 0; i < MENU_LINES; i++)
	{
		cout << this->menu[i] << endl;
	}
}

int Menus::user_input(void)
{

	cin >> input;
	char *schema;
	schema = (char *)malloc(256 * sizeof(char));

	int pwmLevel = 0;
	int chk = 0;

	switch (input)
	{

	case (load_schema):
		cout << "loadSchema" << endl;
		loadSchema(REQ_SCHEMA, sendSchema);
		break;

	case (sel_method):
		string methods_menu = {"Select: 1 setPWM, 2 setRelay, 3 getRelay, 4 getTemp \
								5 setRAMerror "};
		cout << methods_menu << endl;
		cin >> input_method;

		switch (input_method)
		{
		case (setPWM):
			cout << "Set PWM level (0-100%)" << endl;
			cin >> pwmLevel;
			chk = serialize(sendSchema, "setPWM", pwmLevel);
			break;

		case (setRelay):
			cout << "setRelay" << endl;
			break;
		case (getRelay):
			cout << "getRelay" << endl;
			return getRelay;
			break;
		case (getTemp):
			cout << "getTemp" << endl;
			return getRelay;
			break;
		default:
			cout << "Invalid Option\n"
				 << endl;
			break;
		}
		break;

	case (add_error):
		cout << "add_error\n"
			 << endl;
		break;

	case (clear_log):
		cout << "clear_log\n"
			 << endl;
		break;

	case (send_req):
		cout << "send Request" << endl;
		break;

	case (quit):
		cout << "quit" << endl;
		free(schema);
		return quit;
		break;

	default:
		cout << "Invalid Option\n"
			 << endl;
		return -1;
		break;
	}

	free(schema);
	return EXIT_SUCCESS;
}

static void Terminate(int sig, siginfo_t *siginfo, void *context)
{
	{ // Send a cancellation signal to the listener
		syslog(LOG_NOTICE, "Received a terminate signal");
		// Brutally kill the listener
		exit(0);
	}
}

memset(&act, '\0', sizeof(act));

act.sa_sigaction = &Terminate;
act.sa_flags = SA_SIGINFO;

sigaction(SIGHUP, &act, NULL);

#include <sys/types.h>
#include <unistd.h>

#include <syslog.h>
setlogmask(LOG_UPTO(LOG_NOTICE));
openlog("TCP_log", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
syslog(LOG_NOTICE, "Daemon started");

pid_t pid, sid;
pid = fork();

if (pid < 0)
{
	// Bad fork result
	puts("Something went wrong trying to fork");
	exit(EXIT_FAILURE);
}

if (pid > 0)
{
	// Parent process here
	exit(EXIT_SUCCESS);
}
umask(0);
sid = setsid();
if (sid < 0)
{
	// Failed to get good sid
	exit(EXIT_FAILURE);
}
if ((chdir("/")) < 0)
{
	// failed to switch working dir
	exit(EXIT_FAILURE);
}
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
