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
	uint16_t Resolution;
	float SampleTime;

	int16_t LastValue;
	int16_t Delta;

	float Angle;
	float AngularVelocity;
}Encoder_t;

void Encoder_Init(Encoder_t *Encoder, TIM_HandleTypeDef *Tim, uint16_t EncoderResolution, float SampleTime);
void Encoder_AngularVelocity(Encoder_t *Encoder, float *EncoderAngle, float *EncoderAngularVelocity);


#endif /* INC_ENCODER_H_ */
