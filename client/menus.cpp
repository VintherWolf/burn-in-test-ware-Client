/*
 * menus.cpp
 *
 *  Created on: Jun 04, 2020
 *      Author: Daniel K. Vinther Wolf
 */

#include <iostream>
#include <string>
#include <new>

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
	extern char default_ip[16];
	this->input = 0;
	this->input_method = 0;
	this->input_address_info = 0;
	this->servIP = default_ip;
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
	const char *methodsMenu =
		"Select: 1 setPWM, 2 setRelay, 3 getRelay, 4 getTemp"
		" 5 setRAMerror ";
	cin >> input;
	char *schema;
	schema = (char *)malloc(256 * sizeof(char));
	const char *infoMenu = "\nSelect: \n1 set IP Address and Port \n"
						   "2 cancel";
	int pwmLevel = 0;
	int chk = 0;

	switch (input)
	{

	case (load_schema):
		cout << "loadSchema" << endl;
		loadSchema(REQ_SCHEMA, sendSchema);
		break;

	case (sel_method):

		cout << methodsMenu << endl;
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
		case (setRAMerror):
			cout << "setRAMerror" << endl;
			chk = serialize(sendSchema, "setRAMerror", 2);
			break;
		default:
			return 0;
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

	case (info):
		cout << "Current info: " << this->servIP << " " << this->servPort << endl;
		cout << infoMenu << endl;
		cin >> input_address_info;
		switch (input_address_info)
		{

		case (setAddress):
			puts("\nEnter IP-address or q to cancel:");
			char *ip_addr;
			ip_addr = (char *)malloc(16 * sizeof(char));
			ip_addr = this->servIP;

			unsigned short port;
			cin >> this->servIP;
			puts(this->servIP);
			if (strcmp(this->servIP, "q") == 0)
				return 0;

			if (!isValidIpAddress(this->servIP))
			{
				printf("Invalid IP address! (%s)\n", this->servIP);
				this->servIP = ip_addr;
			}
			else
			{
				puts("Enter port:");
				cin >> port;

				if (isdigit(port))
					this->servPort = port;
				else
					puts("Port must be af number");
			}

			break;

		case (cancel):
			return 0;
			break;
		default:
			cout << "Invalid Option\n"
				 << endl;
			return 0;
			break;
		}
		break;

	case (quit):

		free(schema);
		return quit;
		break;

	default:
		cout << "Invalid Option\n"
			 << endl;
		return 0;
		break;
	}

	free(schema);
	free(ip_addr);
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
