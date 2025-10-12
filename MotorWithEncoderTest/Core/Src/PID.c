/*
 * PID.c
 *
 *  Created on: Oct 12, 2025
 *      Author: jakub
 */
#include "PID.h"

void PID_Init(PID_t *Pid, float P, float I, float D, float SampleTime, float MaxValue, float MinValue)
{
	Pid->P = P;
	Pid->I = I;
	Pid->D = D;

	Pid->SampleTime = SampleTime;
	Pid->MaxValue = MaxValue;
	Pid->MinValue = MinValue;

	Pid->Integrator = 0;
	Pid->LastError = 0;
}

