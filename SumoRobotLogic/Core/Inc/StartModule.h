/*
 * SumoRobot.h
 *
 *  Created on: Feb 16, 2026
 *      Author: dryla
 */

#ifndef INC_SUMOROBOT_H_
#define INC_SUMOROBOT_H_

#include <main.h>

#define SIGNAL_START_VALUE GPIO_PIN_SET

typedef enum{
	MODULE_STOP = 0,
	MODULE_COUNTDOWN,
	MODULE_START
}START_MODULE_STATE;


typedef struct{
	START_MODULE_STATE 	State;

	GPIO_TypeDef 	*GpioPort;
	uint16_t 		GpioPin;

	uint32_t TimerDebounce;
	uint32_t LastTick;

	void (*SignalStart)(void);
	void (*SignalReturnToStop)(void);

}StartModule_t;

void StartModule_Init(StartModule_t *StartModule, GPIO_TypeDef *GpioPort ,uint16_t GpioPin, uint32_t TimerDebounce);
void StartModule_Task(StartModule_t *StartModule);

//Callback
void StartModule_StartCallback(StartModule_t *StartModule, void *Callback);
void StartModule_ReturnToStopCallback(StartModule_t *StartModule, void *Callback);





#endif /* INC_SUMOROBOT_H_ */
