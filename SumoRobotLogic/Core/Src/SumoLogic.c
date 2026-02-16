/*
 * SumoLogic.c
 *
 *  Created on: Feb 16, 2026
 *      Author: dryla
 */
#include <SumoRobot.h>

void Init_RobotSignal(RobotState_t *RobotState, GPIO_TypeDef *GpioPort ,uint16_t GpioPin)
{
	RobotState->GpioPort = GpioPort;
	RobotState->GpioPin  = GpioPin;
}

void Check_RobotSignal(RobotState_t *RobotState)
{
	//Check if there was signal to start

	//Wait 5s

	//check if there is still signal to start

	//Change mode to fight
}



