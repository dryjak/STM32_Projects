/*
 * Encoder.c
 *
 *  Created on: Oct 4, 2025
 *      Author: jakub
 */

#include "Encoder.h"

void Encoder_Init(Encoder_t *Encoder, TIM_HandleTypeDef *Tim, uint16_t EncoderResolution, float SampleTime)
{
	Encoder->Tim = Tim;
	Encoder->Resolution = EncoderResolution;
	Encoder->SampleTime = SampleTime;

	Encoder->Delta = 0;
	Encoder->LastValue = 0;

	HAL_TIM_Encoder_Start(Encoder->Tim, TIM_CHANNEL_ALL);
}

void Encoder_AngularVelocity(Encoder_t *Encoder, float *EncoderAngle, float *EncoderAngularVelocity)
{
	int16_t CurrentValue =  __HAL_TIM_GetCounter(Encoder->Tim);
	//*Sum += CurrentValue;
	(Encoder->Delta) = CurrentValue - (Encoder->LastValue);
	if((Encoder->Delta) > (Encoder->Resolution) / 2)
	{
		(Encoder->Delta) -=  (Encoder->Resolution);
	}
	else if((Encoder->Delta) < -(Encoder->Resolution) / 2)
	{
		(Encoder->Delta) += (Encoder->Resolution);
	}

	*EncoderAngle = (360.0 * (Encoder->Delta)) / (Encoder->Resolution);
	*EncoderAngularVelocity = (*EncoderAngle) / (float)(Encoder->SampleTime);
	(Encoder->LastValue) = CurrentValue;
}


