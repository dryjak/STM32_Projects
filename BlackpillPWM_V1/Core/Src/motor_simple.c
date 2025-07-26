/*
 * motor_simple.c
 *
 *  Created on: Jul 26, 2025
 *      Author: jakub
 */

#include "main.h"
#include "tim.h"
#include "motor_simple.h"


MotorsStatus_t Motors_Init(Motors_t *Motor, TIM_HandleTypeDef *timer, GPIO_TypeDef *MotorLeftPort, uint16_t MotorLeftPin, GPIO_TypeDef *MotorRightPort, uint16_t MotorRightPin)
{
	Motor->htim = timer;
	Motor->MotorLeftPort 	= MotorLeftPort;
	Motor->MotorLeftPin 	= MotorLeftPin;
	Motor->MotorRightPort 	= MotorRightPort;
	Motor->MotorRightPin 	= MotorRightPin;
}
