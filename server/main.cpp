/*
 * main.c
 *
 *  Created on: 08 Jun 2020
 *      Author: Daniel K. Vinther Wolf
 */

#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <assert.h>

#include "readtemp.h"
#include "tcpServer.h"
#include "log.h"
#include "jsonhandler.h"
#include "PWM.h"

#define BEAGLEBONE_BLACK 0
#define DOCKERIZED_UBUNTU 1

#ifndef SEL_PLATFORM
#define SEL_PLATFORM (BEAGLEBONE_BLACK && DOCKERIZED_UBUNTU)

//#assert(!SEL_PLATFORM);

#endif

#if BEAGLEBONE_BLACK
// BBB Analog input 0 (P9-39)
#define ADC_P9_39 0

#define REQ_SCHEMA "/usr/bbb-projekter/request.json"
#define RESP_SCHEMA "/usr/bbb-projekter/response.json"

#endif

#define LOGFILE "logfile.log"
// Global handle for the TCP Server and thermometer threads
pthread_t tid;
pthread_t rid;

using namespace exploringBB;

int main()
{

	// log to syslog with LOG_NOTICE
	setlogmask(LOG_UPTO(LOG_NOTICE));

	// Error Message from JSON handler.cpp
	extern char *newError;
	extern char log_filename[64];
	openlog("readtemp", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	// Find the log-entries in /var/log/messages (tail -f)

	syslog(LOG_NOTICE, "Program started by User %d", getuid());

#if BEAGLEBONE_BLACK

	// From the readtemp module
	extern float curTemp;

	// Start the thermometer timer
	if (pthread_create(&rid, NULL, thermometerThread, NULL) != 0)
	{
		syslog(LOG_NOTICE,
			   "Could not install temperature read timer. Terminating!");
		exit(EXIT_FAILURE);
	}
	syslog(LOG_NOTICE, "The temperature timer was started successfully.");

	readThermometer(ADC_P9_39);
	printf("Initial Temperature is: %f\n", curTemp);

#endif

	// Start the TCP thread
	if (pthread_create(&tid, NULL, Th_TCP_RPC, NULL) != 0)
	{
		syslog(LOG_NOTICE,
			   "An error occurred when starting main listener thread.");
		puts("Failed creating listener!");
	}
	syslog(LOG_NOTICE,
		   "The TCP listener thread started successfully. Awaiting your call.");

	initLog("logfile.log");

// Make logentries every 15sec.
#if BEAGLEBONE_BLACK
	for (;;)
	{
		makeLogEntries(LOGFILE, curTemp);

		if (newError != NULL)
		{
			makeLogEntries(LOGFILE, newError);
			newError = NULL;
		}
		usleep(15000000);
		printf("Temperature is: %f\n", curTemp);
	}
#endif

#if DOCKERIZED_UBUNTU
	unsigned log_entries = 1;

	for (;;)
	{
		makeLogEntries(log_filename, 25.0f);
		if (newError != NULL)
		{
			makeLogEntries(log_filename, newError);
			newError = NULL;
		}
		printf("Made log entry in %s (log entry = %u)\n", log_filename, log_entries);
		sleep(15);
		log_entries++;
	}

#endif

	/* Clean up */
	syslog(LOG_NOTICE, "TCP server quitted!");
	closelog();

	printf("\nPROGRAM END\n");

	return 0;
}
