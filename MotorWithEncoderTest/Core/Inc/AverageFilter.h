/*
 * AverageFilter.h
 *
 *  Created on: Oct 15, 2025
 *      Author: jakub
 */

#ifndef INC_AVERAGEFILTER_H_
#define INC_AVERAGEFILTER_H_

#include "main.h"

#define MovingAverageLength 50

typedef struct
{
	float buffer[MovingAverageLength];
	double Sum;
	uint8_t CurrentSample;
	float Output;
}Average_t;

#endif /* INC_AVERAGEFILTER_H_ */
