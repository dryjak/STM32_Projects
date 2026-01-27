/*
 * ButtonStateMashine.h
 *
 *  Created on: Jan 17, 2026
 *      Author: dryla
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_
#include "main.h"

//States of state machine
typedef enum{
	IDLE = 0,
	DEBOUNCE,
	PRESSED,
	LONG_PRESSED,
	REPEAT
}BUTTON_STATE;

//Button Structure
typedef struct {
	BUTTON_STATE 	State;

	GPIO_TypeDef 	*GpioPort;
	uint16_t 		GpioPin;

	uint32_t 		TimerDebounce;	//how much time to wait for debounce
	uint32_t 		TimerLongPress;
	uint32_t		TimerRepeat;
	uint32_t 		LastTick;

	void (*ButtonPressed)(void);
	void (*ButtonLongPressed)(void);
	void (*ButtonRepeat)(void);
	void (*ButtonReturnToIdle)(void);
}Button_t;

//Functions
void ButtonInit(Button_t* Button, GPIO_TypeDef* GpioPort, uint16_t GpioPin, uint32_t TimerDebounce, uint32_t TimerLongPress, uint32_t TimerRepeat);
void ButtonTask (Button_t *Button);

void ButtonSetDebounceTime(Button_t *Button, uint32_t Milliseconds);
void ButtonSetLongPressTime(Button_t *Button, uint32_t Milliseconds);
void ButtonSetRepeatTime(Button_t *Button, uint32_t Milliseconds);

void ButtonRegisterPressCallback(Button_t *Button, void *Callback);
void ButtonRegisterRepeatCallback(Button_t *Button, void *Callback);
void ButtonRegisterLongPressCallback(Button_t *Button, void *Callback);
void ButtonRegisterGoToIdleCallback(Button_t *Button, void *Callback);

#endif /* INC_BUTTON_H_ */
