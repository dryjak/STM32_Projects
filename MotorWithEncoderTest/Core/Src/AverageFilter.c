/*
 * AverageFilter.c
 *
 *  Created on: Oct 15, 2025
 *      Author: jakub
 */


#include "AverageFilter.h"

void Average_Init(Average_t *Average)
{
	Average->Sum = 0;
	Average->Filled = 0;
	Average->CurrentSample = 0;
	Average->Filled = 0;
	for(uint8_t i = 0; i < AVERAGE_SIZE; i++)
	{
		Average->Buffer[i] = 0.0;
	}

}


float Average_Calculate(Average_t *Average, float Input)
{


	Average->Sum -= Average->Buffer[Average->CurrentSample];
	Average->Buffer[Average->CurrentSample] = Input;

	Average->Sum += Input;

	Average->CurrentSample++;

	if (Average->CurrentSample >= AVERAGE_SIZE)
	{
	    Average->CurrentSample = 0;
	    Average->Filled = 1;
	}

	uint8_t Divisor = Average->Filled ? AVERAGE_SIZE : Average->CurrentSample;

	if(Divisor == 0)
		Divisor = 1;

	return (float)Average->Sum / Divisor;

}

