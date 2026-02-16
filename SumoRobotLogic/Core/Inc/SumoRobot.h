/*
 * SumoRobot.h
 *
 *  Created on: Feb 16, 2026
 *      Author: dryla
 */

#ifndef INC_SUMOROBOT_H_
#define INC_SUMOROBOT_H_
#include <main.h>

#define SIGNAL_START_VALUE 1

typedef enum{
	SIGNAL_STOP = 0,
	SIGNAL_DEBOUNCE,
	SIGNAL_START
}SIGNAL_STATE;


typedef struct{
	SIGNAL_STATE 	State;

	GPIO_TypeDef 	*GpioPort;
	uint16_t 		GpioPin;

	uint32_t TimerDebounce;
	uint32_t LastTick;

	void (*SignalStart)(void);
	void (*SignalReturnToStop)(void);

}Signal_t;

void Init_RobotSignal(Signal_t *Signal, GPIO_TypeDef *GpioPort ,uint16_t GpioPin, uint32_t TimerDebounce);
void RobotSignalTask(Signal_t *Signal);





#endif /* INC_SUMOROBOT_H_ */
