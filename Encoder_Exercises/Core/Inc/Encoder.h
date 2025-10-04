/*
 * Encoder.h
 *
 *  Created on: Oct 4, 2025
 *      Author: jakub
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "main.h"

typedef struct
{
	TIM_HandleTypeDef *Tim;
	uint16_t EncoderResolution;
	float SampleTime;

	int16_t CurrentValue;
	int16_t LastValue;
	int16_t Delta;

	float Angle;
	float AngularVelocity;
}Encoder_t;

#endif /* INC_ENCODER_H_ */
