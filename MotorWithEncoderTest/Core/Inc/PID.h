/*
 * PID.h
 *
 *  Created on: Oct 12, 2025
 *      Author: jakub
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"
#include "math.h"
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

	uint8_t Clamp;
}PID_t;

void PID_Init(PID_t *Pid, float P, float I, float D, float SampleTime, float MaxValue, float MinValue);
float PID_Compute(PID_t *Pid, float MeasuredValue, float SetValue);
int8_t Signum(float Value);


#endif /* INC_PID_H_ */
