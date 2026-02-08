/*
 * Button.c
 *
 *  Created on: Jan 17, 2026
 *      Author: dryla
 */

#include "Button.h"

//Init function
void ButtonInit(Button_t* Button, GPIO_TypeDef* GpioPort, uint16_t GpioPin, uint32_t TimerDebounce,
		uint32_t TimerLongPress, uint32_t TimerRepeat)
{
	Button->State = IDLE;
	Button->GpioPort = GpioPort;
	Button->GpioPin  = GpioPin;
	Button->TimerDebounce = TimerDebounce;
	Button->TimerLongPress = TimerLongPress;
	Button->TimerRepeat = TimerRepeat;

}
//Time setting functions
void ButtonSetDebounceTime(Button_t *Button, uint32_t Milliseconds)
{
	Button->TimerDebounce = Milliseconds;
}
void ButtonSetLongPressTime(Button_t *Button, uint32_t Milliseconds)
{
	Button->TimerLongPress = Milliseconds;
}
void ButtonSetRepeatTime(Button_t *Button, uint32_t Milliseconds)
{
	Button->TimerRepeat = Milliseconds;
}
//Register callbacks
void ButtonRegisterPressCallback(Button_t *Button, void *Callback)
{
	Button->ButtonPressed = Callback;
}
void ButtonRegisterLongPressCallback(Button_t *Button, void *Callback)
{
	Button->ButtonLongPressed = Callback;
}
void ButtonRegisterRepeatCallback(Button_t *Button, void *Callback)
{
	Button->ButtonRepeat = Callback;
}
void ButtonRegisterGoToIdleCallback(Button_t *Button, void *Callback)
{
	Button->ButtonReturnToIdle = Callback;
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
		if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))	//when button is pressed
		{
			Button->LastTick = HAL_GetTick();
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

void ButtonPressedRoutine(Button_t *Button)
{
	if(HAL_GetTick() - Button->LastTick >= Button->TimerLongPress)
	{
		Button->State = LONG_PRESSED;
		Button->LastTick = HAL_GetTick();
		if(Button->ButtonLongPressed != NULL)
		{
			Button->ButtonLongPressed();
		}
	}
	else if(GPIO_PIN_SET == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))	//if button is released
	{
		Button->State = IDLE;
		{
			Button->ButtonReturnToIdle();
		}
	}
}

void ButtonLongPressedRoutine (Button_t *Button)
{
	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))
	{
		Button->State = IDLE;
		if(Button->ButtonReturnToIdle != NULL)
		{
			Button->ButtonReturnToIdle();
		}
	}
	else
	{
		if(HAL_GetTick() - Button->LastTick >= Button-> TimerRepeat)
		{
			Button->LastTick = HAL_GetTick();
			Button->State = REPEAT;
			if(Button->ButtonRepeat != NULL)
			{
				Button->ButtonRepeat();
			}
		}
	}
}
void ButtonRepeatRoutine(Button_t *Button)
{
	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))
	{
		Button->State = IDLE;
		if(Button->ButtonReturnToIdle != NULL)
		{
			Button->ButtonReturnToIdle();
		}

	}
	else
	{
		if(HAL_GetTick() - Button->LastTick >= Button->TimerRepeat)
		{
			Button->LastTick = HAL_GetTick();
			if(Button->ButtonRepeat != NULL)
			{
				Button->ButtonRepeat();
			}
		}
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
		ButtonPressedRoutine(Button);
		break;
	case LONG_PRESSED:
		//do LONG PRESSED
		ButtonLongPressedRoutine(Button);
		break;
	case REPEAT:
		ButtonRepeatRoutine(Button);
		break;
	}

}

