/*
 * PID_Constant_Speed.c
 *
 *  Created on: Oct 7, 2025
 *      Author: jakub
 */
#include "PID_Constant_Speed.h"

void Pid_Speed_Init(PID_Speed_t *Pid, float Kp, float Ki, float Kd, float dt)
{
	Pid->Kp = Kp;
	Pid->Ki = Ki;
	Pid->Kd = Kd;

	Pid->dt = dt;
}

float Pid_Speed_Compute(PID_Speed_t *Pid, float Measurement, float ValueSet)
{
	float Error = ValueSet - Measurement;

	float P = Error * Pid->Kp;

	Pid->IntegralValue += Error * Pid->Ki * Pid->dt;

	//Anti windup
	if(Pid->IntegralValue > OUTPUT_MAX)
	{
		Pid->IntegralValue = OUTPUT_MAX;
	}
	else if(Pid->IntegralValue < OUTPUT_MIN)
	{
		Pid->IntegralValue = OUTPUT_MIN;
	}

	float D = ((Error - Pid->LastError) / Pid->dt ) * Pid->Kd;

	float Output = P + Pid->IntegralValue + D;

	if(Output < OUTPUT_MIN)
	{
		Output = OUTPUT_MIN;
	}
	else if (Output > OUTPUT_MAX)
	{
		Output = OUTPUT_MIN;
	}

	return Output;

}

void PidSpeed_Reset(PID_Speed_t *Pid)
{
	Pid->LastError = 0;
	Pid->IntegralValue = 0;
}
