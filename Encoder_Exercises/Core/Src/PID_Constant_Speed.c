/*
 * PID_Constant_Speed.c
 *
 *  Created on: Oct 7, 2025
 *      Author: jakub
 */
#include "PID_Constant_Speed.h"

void Pid_Speed_Init(PID_Speed_t *Pid_Speed, float Kp, float Ki, float Kd, float dt, float OutputMax, float OutputMin)
{
	Pid_Speed->Kp = Kp;
	Pid_Speed->Ki = Ki;
	Pid_Speed->Kd = Kd;

	Pid_Speed->dt = dt;
	Pid_Speed->OutputMin = OutputMin;
	Pid_Speed->OutputMax = OutputMax;
}
