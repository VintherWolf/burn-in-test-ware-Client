/*
 * connection.h
 *
 *  Created on: Mar 5, 2012
 *      Author: klausk
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

struct conParams {
	const char * schema;
	const char * servIP;
	unsigned short servPort;
};


void *ConThread(void *args);


#endif /* CONNECTION_H_ */
