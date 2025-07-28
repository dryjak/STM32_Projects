/*
 * motor_simple.c
 *
 *  Created on: Jul 26, 2025
 *      Author: jakub
 */

#include "main.h"
#include "tim.h"
#include "motor_simple.h"


MotorsStatus_t Motors_Init(Motors_t *Motor, TIM_HandleTypeDef *timer, GPIO_TypeDef *MotorDir1Port, uint16_t MotorDir1Pin)
{
	Motor->htim = timer;
	Motor->MotorLeftPort 	= MotorLeftPort;
	Motor->MotorLeftPin 	= MotorLeftPin;

	return Motors_OK;
}
