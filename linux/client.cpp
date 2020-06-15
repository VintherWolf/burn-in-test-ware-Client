/*
 * client.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: Daniel K. Vinther Wolf
 */

#include <stdio.h>
#include <type_traits>

#include "client.h"

Requester::Requester()
{
	id = 0;
}

Requester::~Requester()
{
}

void Requester::setID(unsigned uint_id)
{

	if (uint_id > 0)
	{
		this->id = uint_id;
	}
	else
	{
		throw "Invalid ID. Must be set to a positive int";
	}
}

unsigned Requester::getID()
{
	return this->id;
}
