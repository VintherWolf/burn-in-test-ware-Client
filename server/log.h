/*
 * log.h
 *
 *  Created on: 8 Jun 2020
 *      Author: Daniel K. Vinther Wolf
 */

#ifndef LOG_H_
#define LOG_H_

void initLog(const char *logfile);
void genTimestamp(char *timestamp, int buflen);
void concatLogfile(char *ts, const char *txt, const char *st, const char *logEntry);
void makeLogEntries(const char *logfile, float tempRead);
void makeLogEntries(const char *logfile, char *errorText);

#endif
