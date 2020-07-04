/*
 * readtemp.c
 *
 *  Created on: 5 Jan 2017
 *      Author: klaus
 */

#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define LDR_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

float curTemp;

#define PCxDBG

void readThermometer (int adc)
{ // returns the input as an int
#ifdef PCDBG
	curTemp = random () % 100;
	syslog (LOG_NOTICE, "readThermometer called.");
#else

	char LDR[512];
	int adc_value;
	sprintf (LDR, "%s%d_raw", LDR_PATH, adc);

	FILE *fs;
	fs = fopen (LDR, "r");
	if (!fs)
	{
		return;
	}
	fscanf (fs, "%d", &adc_value);
	fclose (fs);
	// Calculate the temperature
	// There are 4096 steps in the ADC. Each step equals to 1.80/4096 volts
	float cur_voltage = adc_value * (1.80f / 4096.0f);

	// The LM35DZ outputs 0V + 10mV/C in the range 0 C to 100 C
	// 0V equals 0C and 1.000V equals 100C
	// i.e. the cur_voltage*100 reflects the current temperature
	curTemp = cur_voltage * 100;
#endif
}

void timerHandler (int signum)
{
	const int analogInput = 0;
	if (signum == SIGALRM)
	{
		readThermometer (analogInput);
	}
	else
	{
		syslog (LOG_NOTICE, "timerHandler: Received a signal I do not understand.");
	}
}

void *thermometerThread (void *p)
{
	// Setup timer with a delay of 15 seconds
	struct sigaction sa;
	struct itimerval timer;
	int ret;

#ifdef PCDBG
	// Initialise random generator
	srandom (time (NULL));
#endif
	// Reset curTemp to -1
	//curTemp = -1;
	// Install signal_handler as the signal handler for SIGVTALRM.
	memset (&sa, 0, sizeof(sa));
	sa.sa_handler = &timerHandler;
	if (signal (SIGALRM, timerHandler) == SIG_ERR)
	//	int ret = sigaction (SIGVTALRM, &sa, NULL);
//	if (ret)
	{
		syslog (LOG_NOTICE, "Could not install signal handler\n");
		return NULL;
	}

	// Configure the timer to expire after 15 s
	timer.it_value.tv_sec = 15;
	timer.it_value.tv_usec = 0;
	// ... and every 15 s after that.
	timer.it_interval.tv_sec = 15;
	timer.it_interval.tv_usec = 0;
	// Start a virtual timer. It counts down whenever
	// this process is  executing.
	ret = setitimer (ITIMER_REAL, &timer, NULL);
	if (ret)
	{
		syslog (LOG_NOTICE, "Could not install timer\n");
		return NULL;
	}
	while (1)
	{
		usleep(1000);
	}
	return NULL;
}
