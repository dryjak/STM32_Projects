/*
 * SumoRobotLogic.h
 *
 *  Created on: Feb 15, 2026
 *      Author: dryla
 */

#ifndef INC_SUMOROBOTLOGIC_H_
#define INC_SUMOROBOTLOGIC_H_

typedef enum{
	STOP = 0,
	COUNTDOWN,
	FIGHT
}RobotSignal;

typedef struct{
	RobotSignal SumoSignal;

	uint8_t DistanceSensorRight;
	uint8_t DistanceSensorLeft;
	uint8_t DistanceSensorMiddle;


}RobotState;



#endif /* INC_SUMOROBOTLOGIC_H_ */
