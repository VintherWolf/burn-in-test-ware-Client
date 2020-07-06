

#ifndef SETTINGS_H_
#define SETTINGS_H_

/* MANUALLY SET PLATFORM */
#define BEAGLEBONE_BLACK 0
#define DOCKERIZED_UBUNTU 1

/* Commonly used Paths and Definitions */

#if BEAGLEBONE_BLACK
// BBB Analog input 0 (P9-39)
#define ADC_P9_39 0

#define REQ_SCHEMA "/usr/bbb-projekter/request.json"
#define RESP_SCHEMA "/usr/bbb-projekter/response.json"
#endif

#if DOCKERIZED_UBUNTU

#define REQ_SCHEMA "../jsonschema/request.json"
#define RESP_SCHEMA "/burn-in-app/server/response.json"
#endif

#endif /* COMMON */