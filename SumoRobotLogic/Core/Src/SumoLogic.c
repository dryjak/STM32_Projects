/*
 * SumoLogic.c
 *
 *  Created on: Feb 16, 2026
 *      Author: dryla
 */
#include <SumoRobot.h>



void Init_RobotSignal(Signal_t *Signal, GPIO_TypeDef *GpioPort ,uint16_t GpioPin, uint32_t TimerDebounce)
{
	Signal->State = SIGNAL_STOP;

	Signal->GpioPort = GpioPort;
	Signal->GpioPin  = GpioPin;

	Signal->TimerDebounce = TimerDebounce;

	Signal->SignalStart = NULL;
	Signal->SignalReturnToStop = NULL;
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
		if(Signal->SignalStart != NULL)
		{
			Signal->SignalStart();
		}
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
	if(HAL_GPIO_ReadPin(Signal->GpioPort, Signal->GpioPin) != SIGNAL_START_VALUE)
	{
		Signal->State = SIGNAL_STOP;
	}
	if(Signal->SignalReturnToStop != NULL)
	{
		Signal->SignalReturnToStop();
	}




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



