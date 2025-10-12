/*
 * PID.h
 *
 *  Created on: Oct 12, 2025
 *      Author: jakub
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"

typedef struct
{
	float P;
	float I;
	float D;

	float SampleTime;

	float Integrator;
	float MaxValue;
	float MinValue;

	float LastError;
}PID_t;

void PID_Init(PID_t Pid, float P, float I, float D, float SampleTime, float MaxValue, float MinValue);


#endif /* INC_PID_H_ */
