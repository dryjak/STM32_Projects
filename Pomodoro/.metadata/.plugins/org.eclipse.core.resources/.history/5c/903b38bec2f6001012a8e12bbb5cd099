/*
 * Button.c
 *
 *  Created on: Jan 17, 2026
 *      Author: dryla
 */

#include "Button.h"

//Init function
void ButtonInit(Button_t* Button, GPIO_TypeDef* GpioPort, uint16_t GpioPin, uint32_t TimerDebounce)
{
	Button->State = IDLE;
	Button->GpioPort = GpioPort;
	Button->GpioPin  = GpioPin;
	Button->TimerDebounce = TimerDebounce;
}
//Time setting functions

//Register callbacks
void ButtonRegisterPressCallback(Button_t *Button, void *Callback)
{
	Button->ButtonPressed = Callback;
}


//States of state machine
void ButtonIdleRoutine(Button_t *Button)
{
	//check if button was pressed
	if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))
	{
		Button->State = DEBOUNCE;
		Button->LastTick = HAL_GetTick();
	}
}

void ButtonDebounceRoutine(Button_t *Button)
{
	if(HAL_GetTick() - Button->LastTick >= Button->TimerDebounce)
	{
		if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))
		{
			Button->State = PRESSED;
			if(Button->ButtonPressed != NULL)
			{
				Button->ButtonPressed();
			}
		}
		else
		{
			Button->State = IDLE;
		}
	}
}

void Button_PressedRoutine(Button_t *Button)
{
	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))
	{
		Button->State = IDLE;
	}
}


//State machine
void ButtonTask (Button_t *Button)
{
	switch (Button->State)
	{
	case IDLE:
		//do IDLE
		ButtonIdleRoutine(Button);
		break;
	case DEBOUNCE:
		//do Debounce
		ButtonDebounceRoutine(Button);
		break;
	case PRESSED:
		//do PRESSED;
		Button_PressedRoutine(Button);
		break;
	}

}

