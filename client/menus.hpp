#ifndef SRC_MENUS_HPP_
#define SRC_MENUS_HPP_

#include <string>

#define MENU_LINES 8

enum en_main_menu
{
	load_schema = '1',
	sel_method = '2',
	add_error = '3',
	clear_log = '4',
	send_req = '9',
	info = 'i',
	quit = 'q'
};

enum en_sub_menu1
{
	setPWM = 1,
	setRelay = 2,
	getRelay = 3,
	getTemp = 4,
	setRAMerror = 5
};

enum en_sub_menu2
{
	setAddress = 1,
	cancel = 2,
};

using namespace std;

class Menus
{
public:
	/* Vars */
	string *menu = new string[MENU_LINES];
	char input;
	int input_method;
	int input_address_info;
	char *servIP;
	unsigned short servPort;

	/* Methods */
	Menus(void);
	~Menus(void);

	//private:
	void show_menu(void);
	int user_input(void);
	bool isValidIpAddress(char *st);
};

#endif
