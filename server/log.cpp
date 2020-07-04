/*
 * log.c
 *
 *  Created on: Jun 8, 2020
 *      Author: Daniel K.Vinther Wolf
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define LOGFILES_DIR "/usr/logfiles"
#define LOGFILE "logfile.log"

/*------- LOGFILE ENTRY FORMAT --------*/
// Timestamp
#define TIMEFORMAT "YYYY-MM-DDTHH:MM:SSZ"
#define ISO_TIMEFORMAT "%Y-%m-%dT%H:%M:%S"
// Text
#define WARNING_MESG " High Temperature Warning"
#define ERROR_MESG " High Temperature Error"

// Status
#define ST_PASS ",Off\n"
#define ST_FAIL ",On\n"

#define BUFLEN 512
/*----- LOGFILE ENTRY FORMAT END ------*/

// TEMPERATURE LIMITS (Above WARN_TEMP = Error)
#define LOW_TEMP 45
#define WARN_TEMP 55

void initLog(const char *logfile)
{

	char log[32];
	strncpy(log, logfile, 32);

	FILE *fp;

	if (!mkdir(LOGFILES_DIR, 0777))
	{
		//printf("created %s\n", LOGFILES_DIR);
	}
	else
	{
		//printf("Error, unable to make %s "
		//		"or it allready exists!\n", LOGFILES_DIR);
		;
	}

	// Change the current working directory
	if ((chdir(LOGFILES_DIR)) < 0)
	{
		// Log any failures here
		printf("Could not change working dir to %s", LOGFILES_DIR);
		exit(EXIT_FAILURE);
	}

	fp = fopen(log, "w");

	if (fp == NULL)
	{
		printf("Could not open and write to %s", log);
		exit(EXIT_FAILURE);
	}

	else if (fp < 0)
	{
		// Log any failures here
		printf("Could not open and write to %s", log);
		exit(EXIT_FAILURE);
	}
	fclose(fp);
}

void concatLogfile(char *ts, const char *txt, const char *st, char *logEntry)
{
	strncpy(logEntry, "", BUFLEN - 1);
	char buf[BUFLEN];
	strncpy(buf, ts, BUFLEN - 1);
	// add milisecons to conform to ISO 8601
	strncat(buf, txt, BUFLEN - 1);
	strncat(buf, st, BUFLEN - 1);
	strncpy(logEntry, buf, BUFLEN - 1);
}

void genTimestamp(char *timestamp, int buflen)
{

	// Generate Timestamp for log entry
	time_t rawTimestamp;

	time(&rawTimestamp);

	strftime(timestamp, buflen, ISO_TIMEFORMAT,
			 gmtime(&rawTimestamp));
	strncat(timestamp, ".000Z", buflen);
}

void makeLogEntries(const char *logfile, char *errorText)
{
	FILE *fp;

	char output[BUFLEN];

	// Generate Timestamp for log entry
	//time_t rawTimestamp;

	char timestamp[sizeof TIMEFORMAT];

	//time(&rawTimestamp);
	genTimestamp(timestamp, sizeof TIMEFORMAT);
	//strftime(timestamp, sizeof timestamp, ISO_TIMEFORMAT,
	//		gmtime(&rawTimestamp));

	fp = fopen(logfile, "a");

	if (fp == NULL)
	{
		printf("Could not open and write to %s", logfile);
		exit(EXIT_FAILURE);
	}

	else if (fp < 0)
	{
		printf("Could not open and write to %s", logfile);
		exit(EXIT_FAILURE);
	}

	// Make Log entry based on ADC Temperature reading

	/* Boot Mark */
	puts(errorText);
	if (strcmp(errorText, " ** Hardware Initialization **") == 0)
	{
		concatLogfile(timestamp, errorText, "\n", output);
	}
	else
	{
		/* Standard Failure log entry */
		concatLogfile(timestamp, errorText, ST_FAIL, output);
	}

	fputs(output, fp);

	fclose(fp);
}

void makeLogEntries(const char *logfile, float tempRead)
{
	FILE *fp;

	char output[BUFLEN];
	char timestamp[sizeof TIMEFORMAT];
	genTimestamp(timestamp, sizeof TIMEFORMAT);

	fp = fopen(logfile, "a");

	if (fp == NULL)
	{
		printf("Could not open and write to %s", logfile);
		exit(EXIT_FAILURE);
	}

	else if (fp < 0)
	{
		printf("Could not open and write to %s", logfile);
		exit(EXIT_FAILURE);
	}

	// Make Log entry based on ADC Temperature reading

	/* Warning Off and Error Off */
	if (tempRead <= LOW_TEMP)
	{
		concatLogfile(timestamp, WARNING_MESG, ST_PASS, output);
		fputs(output, fp);

		concatLogfile(timestamp, ERROR_MESG, ST_PASS, output);
		fputs(output, fp);

		/* Warning On and Error Off */
	}
	else if (tempRead > LOW_TEMP && tempRead <= WARN_TEMP)
	{
		concatLogfile(timestamp, WARNING_MESG, ST_FAIL, output);
		fputs(output, fp);
		concatLogfile(timestamp, ERROR_MESG, ST_PASS, output);
		fputs(output, fp);

		/* Warning On and Error On */
	}
	else
	{
		concatLogfile(timestamp, WARNING_MESG, ST_FAIL, output);
		fputs(output, fp);
		concatLogfile(timestamp, ERROR_MESG, ST_FAIL, output);
		fputs(output, fp);
	}

	fclose(fp);
}
