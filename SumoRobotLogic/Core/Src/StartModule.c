/*
 * SumoLogic.c
 *
 *  Created on: Feb 16, 2026
 *      Author: dryla
 */
#include <StartModule.h>

static void StartModule_CountdownRoute(StartModule_t *StartModule);
static void StartModule_StopRoute(StartModule_t *StartModule);
static void StartModule_StartRoute(StartModule_t *StartModule);

//Initialize function
void StartModule_Init(StartModule_t *StartModule, GPIO_TypeDef *GpioPort ,uint16_t GpioPin, uint32_t TimerDebounce)
{
	StartModule->State = MODULE_STOP;

	StartModule->GpioPort = GpioPort;
	StartModule->GpioPin  = GpioPin;

	StartModule->TimerDebounce = TimerDebounce;

	StartModule->SignalStart = NULL;
	StartModule->SignalReturnToStop = NULL;
}

void StartModule_StopRoute(StartModule_t *StartModule)
{
	if(HAL_GPIO_ReadPin(StartModule->GpioPort, StartModule->GpioPin) == SIGNAL_START_VALUE)
	{
		StartModule->LastTick = HAL_GetTick();
		StartModule->State = MODULE_COUNTDOWN;
	}
}

void StartModule_CountdownRoute(StartModule_t *StartModule)
{
	if(HAL_GetTick() - StartModule->LastTick >= StartModule->TimerDebounce)
	{
		//Go to the start state
		StartModule->State = MODULE_START;
		if(StartModule->SignalStart != NULL)
		{
			StartModule->SignalStart();
		}
	}
	else if(HAL_GPIO_ReadPin(StartModule->GpioPort, StartModule->GpioPin) != SIGNAL_START_VALUE)
	{
		//Go back to stop state
		StartModule->State = MODULE_STOP;
		if(StartModule->SignalReturnToStop != NULL)
		{
			StartModule->SignalReturnToStop();
		}
	}
}

void StartModule_StartRoute(StartModule_t *StartModule)
{
	if(HAL_GPIO_ReadPin(StartModule->GpioPort, StartModule->GpioPin) != SIGNAL_START_VALUE)
	{
		StartModule->State = MODULE_STOP;
		if(StartModule->SignalReturnToStop != NULL)
		{
			StartModule->SignalReturnToStop();
		}
	}
}

//Callback
void StartModule_StartCallback(StartModule_t *StartModule, void *Callback)
{
	StartModule->SignalStart = Callback;
}

void StartModule_ReturnToStopCallback(StartModule_t *StartModule, void *Callback)
{
	StartModule->SignalReturnToStop = Callback;
}

//Finite state machine
void StartModule_Task(StartModule_t *StartModule)
{
	//Check if there was signal to start
	switch (StartModule->State)
	{
	case MODULE_STOP:
		StartModule_StopRoute(StartModule);
		break;
	case MODULE_COUNTDOWN:
		//Wait e.g. 5s
		StartModule_CountdownRoute(StartModule);
		break;
	case MODULE_START:
		//Check if there is still signal to start
		//Change mode to fight
		StartModule_StartRoute(StartModule);
		break;
	}
}



