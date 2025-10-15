/*
 * AverageFilter.c
 *
 *  Created on: Oct 15, 2025
 *      Author: jakub
 */


#include "AverageFilter.h"

void Average_Init(Average_t *Average, uint8_t Size)
{

	Average->Size = Size;

	for(uint8_t i = 0; i < Average->Sum; i++)
	{
		Average->Buffer[i] = 0.0f;
	}

}


void Average_Calculate(Average_t *Average, float Input, float *Output)
{

}

