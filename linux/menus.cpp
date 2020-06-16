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

Menus::Menus() {

	this->input = 0;
	this->input_method = 0;


	for (int i = 0; i < MENU_LINES; i++) {
		this->menu[i] = { " " };
	}

	this->menu[0] = { "Select:" };
	this->menu[1] = { "1 Load Schema" };
	this->menu[2] = { "2 Select Method" };
	this->menu[3] = { "3 Add Error to Log" };
	this->menu[4] = { "4 Clear Log" };
	this->menu[5] = { "9 Send Request" };
	this->menu[6] = { "q quit" };

}

Menus::~Menus() {

	delete[] menu;
}

void Menus::show_menu(void) {
	puts("");
	for (int i = 0; i < MENU_LINES; i++) {
		cout << this->menu[i] << endl;
	}
}

int Menus::user_input(void) {

	cin >> input;
	char * schema;
	schema = (char*) malloc(256 * sizeof (char));

	int pwmLevel = 0;
	int chk = 0;

	switch (input) {

	case (load_schema):
		cout << "loadSchema" << endl;
		loadSchema(REQ_SCHEMA, sendSchema);
		break;

	case (sel_method):
		cout << "Select: 1 setPWM, 2 setRelay, 3 getRelay, 4 getTemp" << endl;
		cin >> input_method;

		switch (input_method) {
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
			cout << "Invalid Option\n" << endl;
			break;
		}
		break;

	case (add_error):
		cout << "add_error\n" << endl;
		break;

	case (clear_log):
		cout << "clear_log\n" << endl;
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
		cout << "Invalid Option\n" << endl;
		return -1;
		break;
	}

	free(schema);
	return EXIT_SUCCESS;

}

