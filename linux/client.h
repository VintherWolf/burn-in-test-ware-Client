/*
 * client.h
 *
 * Created on: Apr 11, 2020
 * Author: Daniel K. Vinther Wolf
 *
 */

#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

#include <string.h>

class Requester
{
public:
	Requester();
	virtual ~Requester();
	void setID(unsigned uint_id);
	unsigned getID();

private:
	unsigned id;
};

#endif /* SRC_CLIENT_H_ */
