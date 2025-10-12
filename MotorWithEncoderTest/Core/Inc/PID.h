/*
 * PID.h
 *
 *  Created on: Oct 12, 2025
 *      Author: jakub
 */

#ifndef INC_PID_H_
#define INC_PID_H_

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


#endif /* INC_PID_H_ */
