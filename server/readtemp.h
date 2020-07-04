/*
 * readtemp.h
 *
 *  Created on: 5 Jan 2017
 *      Author: klaus
 */

#ifndef READTEMP_H_
#define READTEMP_H_

void *thermometerThread (void *p);
void readThermometer (int adc);

#endif /* READTEMP_H_ */
