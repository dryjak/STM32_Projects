/*
 * PID_Constant_Speed.c
 *
 *  Created on: Oct 7, 2025
 *      Author: jakub
 */
#include "PID_Constant_Speed.h"

void Pid_Speed_Init(PID_Speed_t *Pid, float Kp, float Ki, float Kd, float dt, float OutputMax, float OutputMin)
{
	Pid->Kp = Kp;
	Pid->Ki = Ki;
	Pid->Kd = Kd;

	Pid->dt = dt;
	Pid->OutputMin = OutputMin;
	Pid->OutputMax = OutputMax;
}

float Pid_Speed_Compute(PID_Speed_t *Pid, float Measurement, float ValueSet)
{
	float Error = ValueSet - Measurement;

	float P = Error * Pid->Kp;

	Pid->IntegralValue += Error * Pid->Ki * Pid->dt;

	//Anti windup
	if(Pid->IntegralValue > Pid->OutputMax)
	{
		Pid->IntegralValue = Pid->OutputMax;
	}
	else if(Pid->IntegralValue < Pid->OutputMin)
	{
		Pid->IntegralValue = Pid->OutputMin;
	}

	float D = ((Error - Pid->LastError) / Pid->dt ) * Pid->Kd;

	float Output = P + Pid->IntegralValue + D;

	if(Output < Pid->OutputMin)
	{
		Output = Pid->OutputMin;
	}
	else if (Output > Pid->OutputMax)
	{
		Output = Pid->OutputMax;
	}

	return Output;

}

void PidSpeed_Reset(PID_Speed_t *Pid)
{
	Pid->LastError = 0;
	Pid->IntegralValue = 0;
}
