/*
 * AverageFilter.h
 *
 *  Created on: Oct 15, 2025
 *      Author: jakub
 */

#ifndef INC_AVERAGEFILTER_H_
#define INC_AVERAGEFILTER_H_

#include "main.h"

#define AVERAGE_SIZE 50

typedef struct
{
	float Buffer[AVERAGE_SIZE];
	double Sum;
	uint8_t CurrentSample;
	uint8_t Filled;
	uint8_t Size;
}Average_t;

void Average_Init(Average_t *Average);
void Average_Calculate(Average_t *Average, float Input, float *Output);

#endif /* INC_AVERAGEFILTER_H_ */
