/*
 * Encoder.c
 *
 *  Created on: Oct 4, 2025
 *      Author: jakub
 */

#include "Encoder.h"

void Init_Encoder(Encoder_t *Encoder, TIM_HandleTypeDef *Tim, uint16_t EncoderResolution, float SampleTime)
{
	Encoder->Tim = Tim;
	Encoder->EncoderResolution = EncoderResolution;
	Encoder->SampleTime = SampleTime;

	HAL_TIM_Encoder_Start(Encoder->Tim, TIM_CHANNEL_ALL);
}
