/*
 * PID_Constant_Speed.h
 *
 *  Created on: Oct 7, 2025
 *      Author: jakub
 */

#ifndef INC_PID_CONSTANT_SPEED_H_
#define INC_PID_CONSTANT_SPEED_H_

typedef struct
{
	float Kp;
	float Ki;
	float Kd;

	float dt;
	float IntegralValue;
	float LastError;

	float OutputMax;
	float OutputMin;

}PID_Speed_t;

void Pid_Speed_Init(PID_Speed_t Pid_Speed, float Kp, float Ki, float Kd, float dt, float Integral, float LastError, float OutputMax, float OutputMin);

#endif /* INC_PID_CONSTANT_SPEED_H_ */
