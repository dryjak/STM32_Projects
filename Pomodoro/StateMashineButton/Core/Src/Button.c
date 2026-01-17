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
void *ButtonPressed(void)
{

}


//States of state machine

//State machine

