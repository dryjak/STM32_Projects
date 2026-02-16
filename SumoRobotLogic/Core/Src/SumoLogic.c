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

void RobotSignalTask(RobotState_t *RobotState)
{
	//Check if there was signal to start
	switch (RobotState->SumoSignal)
	{
	case STOP:
		break;
	case COUNTDOWN:
		//Wait 5s
		break;
	case FIGHT:
		//Check if there is still signal to start
		//Change mode to fight
		break;
	}
}



