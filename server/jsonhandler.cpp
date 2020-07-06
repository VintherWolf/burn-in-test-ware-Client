/*
 * jsonhandler.c
 *
 *  Created on: Jun 8, 2020
 *      Author:  Daniel K. Vinther Wolf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../include/rjson/document.h"
#include "../include/rjson/prettywriter.h"
#include "../include/rjson/writer.h"
#include "../include/rjson/stringbuffer.h"
#include "../include/rjson/error/en.h"

#include "PWM.h"
#include "log.h"
#include "settings.h"

#define BUFLEN 256

// BBB PWM ch. 0
#define PWM_P9_42 "pwmchip0/pwm-0:0"

using namespace rapidjson;
using namespace std;
using namespace exploringBB;

char newError[256];

void loadSchema(const char *schema, char *output)
{

	FILE *fp;
	char buf[BUFLEN];

	fp = fopen(schema, "r");

	if (fp == NULL)
	{
		printf("Could not use %s", schema);
		exit(EXIT_FAILURE);
	}

	else if (fp < 0)
	{
		printf("Could not use %s", schema);
		exit(EXIT_FAILURE);
	}

	int c;
	int i = 0;
	do
	{
		c = fgetc(fp);
		if (isspace(c))
			continue;
		buf[i] = c;
		i++;
	} while (c != EOF);
	buf[i - 1] = '\0';
	strncpy(output, buf, sizeof buf);

	fclose(fp);
}

int valReq(const char *schema)
{

	Document rpcdoc;
	rpcdoc.Parse(schema);
	int check = 0;
	int iD = 0;

	// Check for parse errors:

	//try {
	//	rpcdoc.HasParseError();
	//}
	//catch (...) {
	//	puts("Invalid JSON schema received!");
	//return -1;
	//printf(stderr, "\nError(offset %u): %s\n",
	//		(unsigned)rpcdoc.GetErrorOffset(),
	//		GetParseError_En(rpcdoc.GetParseError()));
	//}

	if (rpcdoc.HasParseError())
	{
		{
			fprintf(stderr, "\nError(offset %u): %s\n",
					(unsigned)rpcdoc.GetErrorOffset(),
					GetParseError_En(rpcdoc.GetParseError()));
			return -1;
		}
	}

	if (rpcdoc.HasMember("jsonrpc"))
	{
		//printf("jsonrpc = %s\n", rpcdoc["jsonrpc"].GetString());
		check += 1000;
	}

	// Check: is RPC Version 2.0
	if (rpcdoc["jsonrpc"] == "2.0")
	{
		check += 1000;
	}
	else
	{
		printf("NOT OK. RPC-Schema Version (%s) is NOT 2.0\n ",
			   rpcdoc["jsonrpc"].GetString());
	}
	if (rpcdoc.HasMember("method"))
	{
		//printf("method = %s\n", rpcdoc["method"].GetString());
		check += 1000;

		// Check: Method is known
		if (rpcdoc["method"] == "<method>")
		{
			// default value
			;
		}
		else if (rpcdoc["method"] == "setPWM")
		{
			check += 100;
			if (rpcdoc["params"].IsInt())
				;
			else
			{
				puts("params is not type Int!");
				return -1;
			}
		}
		else if (rpcdoc["method"] == "setRelay")
		{
			check += 200;
		}
		else if (rpcdoc["method"] == "getRelay")
		{
			check += 300;
		}
		else if (rpcdoc["method"] == "getTemp")
		{
			check += 400;
		}
		else if (rpcdoc["method"] == "setRAMerror")
		{
			check += 500;
		}
		else if (rpcdoc["method"] == "setBootMark")
		{
			check += 600;
		}
		else
		{
			// unknown value
			return -1;
		}
	}
	if (rpcdoc.HasMember("params"))
	{
		check += 1000;
	}

	if (rpcdoc.HasMember("result"))
	{
		// Note 10 times more than other Members
		check += 10000;
	}

	if (rpcdoc.HasMember("id"))
	{
		check += 1000;
	}

	// Check: ID
	if (rpcdoc["id"] == 0)
	{
		// default value
		;
	}
	else
	{

		iD = rpcdoc["id"].GetInt();
	}
	if (iD >= 0 && iD < 10)
		check += iD;
	else
		// Invalid ID:
		return -1;

	return check;
}

int serialize(char *schema, char *method, int param)
{
	Document rpcdoc;

	rpcdoc.Parse(schema);

	if ((valReq(schema)) < 5000)
	{
		puts("setMethod: Invalid Schema!");
		return -1;
	}
	//int method = ((req - 5000) / 100);

	if (strcmp(method, "setPWM") == 0)
	{
		rpcdoc["method"].SetString("setPWM");
		if (param < 100)
		{
			rpcdoc["params"].SetInt(param);
		}
		else
		{
			puts("Duty-Cycle value is bigger than 100%!");
			return -1;
		}
	}
	else if (strcmp(method, "setRelay") == 0)
	{
		rpcdoc["method"].SetString("setRelay");
		rpcdoc["params"].SetInt(param);
	}
	else if (strcmp(method, "getRelay") == 0)
	{
		rpcdoc["method"].SetString("getRelay");
		rpcdoc["params"].SetInt(param);
	}
	else
	{
		rpcdoc["method"].SetString("");
		puts("Failed to Serialize!");
		return -1;
	}

	StringBuffer sb;

	Writer<StringBuffer> writer(sb);
	rpcdoc.Accept(writer);
	const char *output = sb.GetString();

	strncpy(schema, output, BUFLEN);

	printf("Sending: %s\n", output);

	return 0;
}

void startPWM(void)
{
	PWM pwm(PWM_P9_42);

	pwm.setFrequency(100);
	pwm.setDutyCycle(5.0f);
	pwm.setPolarity(PWM::ACTIVE_HIGH);
	pwm.run();
}

int handleReq(const char *str_schema)
{

#if BEAGLEBONE_BLACK
	PWM pwm(PWM_P9_42);

	pwm.setFrequency(100);
	pwm.setDutyCycle(5.0f);
	pwm.setPolarity(PWM::ACTIVE_HIGH);
	pwm.run();
#endif

	Document rpcdoc;
	rpcdoc.Parse(str_schema);

	int params = 0;

	int req = 0;
	if ((req = valReq(str_schema)) < 5000)
	{
		puts("setMethod: Invalid Schema!");
		return -1;
	}

	int method = ((req - 5000) / 100);
	if (method)
	{
		switch (method)
		{

		case (1):
			// method = setPWM
			params = rpcdoc["params"].GetInt();
#if BEAGLEBONE_BLACK
			pwm.setDutyCycle((float)params);
#endif
			printf("setPWM to %d\n", params);

			break;
		case (2):
			// method = setRelay
			params = rpcdoc["params"].GetInt();

			break;
		case (3):
			// method = getRelay
			params = rpcdoc["params"].GetInt();
			printf("getRelay %d\n", params);
			break;
		case (4):
// method = getTemp
#if BEAGLEBONE_BLACK
			extern float curTemp;
			params = (int)curTemp;
#endif
			printf("getTemp %d\n", params);
			break;

		case (5):
			// method = setRAMerror
			printf("setRAMerror\n");
			strncpy(newError, " RAM Test Fail#Ssram Ctrl#RxTx Ctrl", 64);

			break;

		case (6):
			// method = setBootMark
			printf("setBootMark\n");
			strncpy(newError, " ** Hardware Initialization **", 64);
			break;

		default:
			// unknown method;
			printf("Unknown Method\n");
			return -1;
			break;
		}
	}

	return params;
}

int getID(char *schema)
{

	Document rpcdoc;
	rpcdoc.Parse(schema);

	int iD = 0;

	int req = 0;
	if ((req = valReq(schema)) < 5000)
	{
		puts("setMethod: Invalid Schema!");
		return -1;
	}

	iD = rpcdoc["id"].GetInt();
	return iD;
}

int handleResp(char *schema, int result, int id)
{

	Document rpcdoc;
	rpcdoc.Parse(schema);
	int req = -1;
	if ((req = valReq(schema)) < 5000)
	{
		puts("setMethod: Invalid Schema!");
		return -1;
	}

	rpcdoc["result"].SetInt(result);
	printf("Result to %d\n", result);
	rpcdoc["id"].SetInt(id);

	StringBuffer sb;

	Writer<StringBuffer> writer(sb);
	rpcdoc.Accept(writer);
	const char *output = sb.GetString();

	strncpy(schema, output, BUFLEN);

	printf("Sending: %s\n", output);

	return 0;
}
