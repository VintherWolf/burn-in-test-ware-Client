/*
 * menus.cpp
 *
 *  Created on: Jun 04, 2020
 *      Author: Daniel K. Vinther Wolf
 */

#include <iostream>
#include <string>
#include <new>
#include <unistd.h>

#include "menus.hpp"
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
	this->servIP = {"127.0.0.1"};
	this->servPort = 1955;

	for (int i = 0; i < MENU_LINES; i++)
	{
		this->menu[i] = {" "};
	}

	this->menu[0] = {"Select:"};
	this->menu[1] = {"1 Load Schema"};
	this->menu[2] = {"2 Methods"};
	this->menu[3] = {" "};
	this->menu[4] = {" "};
	this->menu[5] = {"i Info"};
	this->menu[6] = {"q quit"};
}

Menus::~Menus()
{
	delete[] menu;
}

void Menus::show_main_menu(void)
{
	puts("");
	for (int i = 0; i < MENU_LINES; i++)
	{
		cout << this->menu[i] << endl;
	}
}

int Menus::main_menu(void)
{
	/* MENUS */

	// allocate memory for "load Schema" :

	char selection;
	show_main_menu();
	cin >> selection;
	switch (selection)
	{

	case (load_schema):
		cout << "loadSchema" << endl;
		loadSchema(REQ_SCHEMA, sendSchema);
		break;

	case (sel_method):
		sub_methods();
		// sel_method
		break;

	case (info):
		sub_info();
		break;

	case (quit):

			return quit;
		break;

	default:
		cout << "Invalid Option\n"
			 << endl;
		break;
	}

	cin.clear();
	return EXIT_SUCCESS;
}

bool Menus::isValidIpAddress(char *st)
{
	int num, i, len;
	char *ch;

	//counting number of quads present in a given IP address
	int quadsCnt = 0;

	printf("Split IP: \"%s\"\n", st);

	len = strlen(st);

	//  Check if the string is valid
	if (len < 7 || len > 15)
		return false;

	ch = strtok(st, ".");

	while (ch != NULL)
	{
		quadsCnt++;
		printf("Quald %d is %s\n", quadsCnt, ch);

		num = 0;
		i = 0;

		//  Get the current token and convert to an integer value
		while (ch[i] != '\0')
		{
			num = num * 10;
			num = num + (ch[i] - '0');
			i++;
		}

		if (num < 0 || num > 255)
		{
			printf("Not a valid ip\n");
			return false;
		}

		if ((quadsCnt == 1 && num == 0) || (quadsCnt == 4 && num == 0))
		{
			printf("Not a valid ip, quad: %d AND/OR quad:%d is zero\n",
				   quadsCnt, quadsCnt);
			return false;
		}

		ch = strtok(NULL, ".");
	}

	//  Check the address string, should be n.n.n.n format
	if (quadsCnt != 4)
	{
		return false;
	}

	//  Looks like a valid IP address
	return true;
}

void Menus::sub_methods(void)
{
	int selection;
	const char *methodsMenu =
		"Select: 1 setPWM, 2 setRelay, 3 getRelay, 4 getTemp"
		" 5 setRAMerror ";
	int pwmLevel = 0;
	int chk = 0;
	cout << methodsMenu << endl;
	cin >> selection;

	switch (selection)
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

		break;
	case (getTemp):
		cout << "getTemp" << endl;

		break;
	case (setRAMerror):
		cout << "setRAMerror" << endl;
		chk = serialize(sendSchema, "setRAMerror", 2);
		break;
	default:
		cout << "Invalid Option\n"
			 << endl;
		break;
	}
	cin.clear();
}

void Menus::sub_info(void)
{
	int selection;

	// Menu text:
	const char *infoMenu = "\nSelect: \n1 set IP Address and Port \n"
						   "2 cancel";

	cout << "Current info: " << this->servIP << " " << this->servPort << endl;
	cout << infoMenu << endl;
	cin >> selection;

	switch (selection)
	{

	case (setAddress):
		puts("\nEnter IP-address or q to cancel:");

		char inBuf[16];
		char ipAddr[16];
		unsigned short port;

		cin >> inBuf;

		if (strcmp(inBuf, "q") == 0)
			break;

		strncpy(ipAddr, inBuf, 16);

		if (!isValidIpAddress(inBuf))
		{
			printf("Invalid IP address! (%s)\n", ipAddr);
		}
		else
		{
			printf("Entered IP address is: %s\n", ipAddr);
			puts("Enter port:");
			cin >> port;

			if (!isalpha(port))
			{
				this->servIP = ipAddr;
				this->servPort = port;
			}
			else
				puts("Port must be af number");
		}

		break;

	case (cancel):

		break;
	default:
		cout << "Invalid Option\n"
			 << endl;
		break;
	}
	cin.clear();
}