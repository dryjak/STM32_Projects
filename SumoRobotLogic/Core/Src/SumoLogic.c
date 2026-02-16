/*
 * SumoLogic.c
 *
 *  Created on: Feb 16, 2026
 *      Author: dryla
 */
#include <SumoRobot.h>



void Init_RobotSignal(Signal_t *Signal, GPIO_TypeDef *GpioPort ,uint16_t GpioPin)
{
	Signal->GpioPort = GpioPort;
	Signal->GpioPin  = GpioPin;
}

void RobotStopRoute(Signal_t *Signal)
{
	if(HAL_GPIO_ReadPin(Signal->GpioPort, Signal->GpioPin) == SIGNAL_START_VALUE)
	{
		Signal->LastTick = HAL_GetTick();
		Signal->State = SIGNAL_DEBOUNCE;
	}
}

void RobotCountdownRoute(Signal_t *Signal)
{
	if(HAL_GetTick() - Signal->LastTick >= Signal->TimerDebounce)
	{
		//Go to the fight state
		Signal->State = SIGNAL_START;
	}
	else
	{
		//Go back to stop state
		Signal->State = SIGNAL_STOP;
		if(Signal->SignalReturnToStop != NULL)
		{
			Signal->SignalReturnToStop();
		}
	}
}

void RobotFightRoute(Signal_t *Signal)
{
	if(Signal->SignalStart == NULL)
	{
		return;
	}

	Signal->SignalStart();
}

//Callback
void SignalStartCallback(Signal_t *Signal, void *Callback)
{
	Signal->SignalStart = Callback;
}

void SignalReturnToStopCallback(Signal_t *Signal, void *Callback)
{
	Signal->SignalReturnToStop = Callback;
}

//Finite state machine
void RobotSignalTask(Signal_t *Signal)
{
	//Check if there was signal to start
	switch (Signal->State)
	{
	case SIGNAL_STOP:
		break;
	case SIGNAL_DEBOUNCE:
		//Wait 5s
		break;
	case SIGNAL_START:
		//Check if there is still signal to start
		//Change mode to fight
		break;
	}
}



