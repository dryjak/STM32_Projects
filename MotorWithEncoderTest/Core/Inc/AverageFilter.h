/*
 * AverageFilter.h
 *
 *  Created on: Oct 15, 2025
 *      Author: jakub
 */

#ifndef INC_AVERAGEFILTER_H_
#define INC_AVERAGEFILTER_H_

#include "main.h"

#define SIZE 50

typedef struct
{
	float Buffer[SIZE];
	double Sum;
	uint8_t CurrentSample;
	uint8_t Filled;
}Average_t;

void Average_Init(Average_t *Average, uint8_t Sum);
void Average_Calculate(Average_t *Average, float Input, float *Output);

#endif /* INC_AVERAGEFILTER_H_ */
