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
	Pid->Clamp = 0;
}

float PID_Compute(PID_t *Pid, float MeasuredValue, float SetValue)
{
	float Error = SetValue - MeasuredValue;

	//proportional value
	float P = Pid->P * Error;

	//Integrator value
	if(Pid->Clamp == 0)
	{
	Pid->Integrator += Pid->SampleTime * Pid->I * Error;
	}


	//Deriative value
	float D = ((Error - Pid->LastError) / Pid->SampleTime) * Pid->D;

	float Output = P + Pid->Integrator + D;
	float OutputLast = Output;

	//checking limits
	if (Output > Pid->MaxValue)
	{
		Output = Pid->MaxValue;
	}
	else if (Output < Pid->MinValue)
	{
		Output = Pid->MinValue;
	}

	uint8_t ClampigSaturationCheck = (Output != OutputLast) ? 1 : 0;

	int8_t ErrorSign = Signum(Error);
	int8_t OutputSign = Signum(Output);

	if ((ErrorSign == OutputSign) && (ClampigSaturationCheck == 1))
	{
		Pid->Clamp = 1;
	}
	else
	{
		Pid->Clamp = 0;
	}

	//Update LastError
	Pid->LastError = Error;

	return Output;

}

int8_t Signum(float Value)
{
	if (Value > 0.0) return 1;
	if (Value < 0.0) return -1;
	return 0;
}


