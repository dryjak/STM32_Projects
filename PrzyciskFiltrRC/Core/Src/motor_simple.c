/*
 * motor_simple.c
 *
 *  Created on: Jul 26, 2025
 *      Author: jakub
 */

#include "main.h"
#include "tim.h"
#include "motor_simple.h"

#define DIR(a) ((a)>(0)?(1):(0))

MotorStatus_t Motor_Ride(Motor_t *Motor)
{

	HAL_GPIO_WritePin(Motor->MotorDir1Port, Motor->MotorDir1Pin, DIR(Motor->Direction));
	HAL_GPIO_WritePin(Motor->MotorDir1Port, Motor->MotorDir2Pin, DIR(Motor->Direction));
	__HAL_TIM_SET_COMPARE(Motor->htim, Motor->Channel, Motor->MotorPWM);

	return MOTOR_OK;
}

MotorStatus_t Motor_Init(Motor_t *Motor, TIM_HandleTypeDef *Timer, uint32_t TimerChannel, uint16_t PWM, GPIO_TypeDef *Dir1Port, uint16_t Dir1Pin, GPIO_TypeDef *Dir2Port, uint16_t Dir2Pin)
{
	Motor->htim = Timer;
	Motor->Channel = TimerChannel;

	Motor->MotorPWM = PWM;

	Motor->MotorDir1Port = Dir1Port;
	Motor->MotorDir1Pin = Dir1Pin;
	Motor->MotorDir2Port = Dir2Port;
	Motor->MotorDir2Pin = Dir2Pin;

	return MOTOR_OK;
}
