/*
 * SumoRobot.h
 *
 *  Created on: Feb 16, 2026
 *      Author: dryla
 */

#ifndef INC_SUMOROBOT_H_
#define INC_SUMOROBOT_H_
#include <main.h>

typedef enum{
	STOP = 0,
	COUNTDOWN,
	FIGHT
}RobotSignal;

typedef struct{
	RobotSignal 	SumoSignal;
	GPIO_TypeDef 	*GpioPort;
	uint16_t 		GpioPin;

}RobotState_t;




#endif /* INC_SUMOROBOT_H_ */
