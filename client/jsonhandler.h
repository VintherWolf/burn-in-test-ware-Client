/*
 * jsonhandler.h
 *
 *  Created on: Jun 8, 2020
 *      Author:  Daniel K. Vinther Wolf
 */

#ifndef BBB_JSONHANDLER_H_
#define BBB_JSONHANDLER_H_

void loadSchema(const char * schema, char * output);
int valReq(const char * schema);
int serialize(char *schema, const char * method, int param);
int getID(char * schema);
int handleReq(const char *str_schema);
int handleResp(char * str_schema, int result, int id);

#endif /* BBB_JSONHANDLER_H_ */
