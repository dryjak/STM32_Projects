/*
 * SumoRobot.h
 *
 *  Created on: Feb 19, 2026
 *      Author: dryla
 */

#ifndef INC_SUMOROBOT_H_
#define INC_SUMOROBOT_H_
#include <main.h>

typedef struct{
	GPIO_TypeDef *FlorPortL;	uint16_t FlorPinL;
	GPIO_TypeDef *FlorPortR;	uint16_t FlorPinR;

	GPIO_TypeDef *DistancePortM;	uint16_t DistancePinM;
	GPIO_TypeDef *DistancePortL;	uint16_t DistancePinL;
	GPIO_TypeDef *DistancePortR;	uint16_t DistancePinR;

	//I have used 3 pin dip switch
	GPIO_TypeDef *DipSwitchPort1;	uint16_t DipSwitchPin1;
	GPIO_TypeDef *DipSwitchPort2;	uint16_t DipSwitchPin2;
	GPIO_TypeDef *DipSwitchPort3;	uint16_t DipSwitchPin3;

	//place for motors
}SumoRobotHardware_t;

typedef union{
	struct{
		uint8_t FlorL : 1;		//1 - black, 0 - white
		uint8_t FlorR : 1;
		uint8_t DistanceM : 1;	//1 - not detected, 0 - detected
		uint8_t DistanceR : 1;
		uint8_t DistanceL : 1;
		uint8_t EmptySpace: 3;
	};
	uint8_t AllSensors;
}SumoRobotSensors_t	;

typedef enum{
	MOVE_STOP = 0,
	MOVE_FORWARD,
	MOVE_TURN_LEFT,
	MOVE_TURN_RIGHT,
	MOVE_TURN_SLIGHT_LEFT,
	MOVE_TURN_SLIGHT_RIGHT,
	MOVE_BACKWARD,
}SumoRobotMove_t;

typedef struct{
	uint8_t Tactics;	//reading from dip switch
	SumoRobotSensors_t Sensors;
	SumoRobotMove_t Move;
	SumoRobotHardware_t Hardware;

}SumoRobot_t;


//Initialize function
void SumoRobotInit(SumoRobot_t *SumoRobot,
GPIO_TypeDef *FlorPortL,	uint16_t FlorPinL,
GPIO_TypeDef *FlorPortR,	uint16_t FlorPinR,

GPIO_TypeDef *DistancePortM,	uint16_t DistancePinM,
GPIO_TypeDef *DistancePortL,	uint16_t DistancePinL,
GPIO_TypeDef *DistancePortR,	uint16_t DistancePinR,

//I have used 3 pin dip switch
GPIO_TypeDef *DipSwitchPort1,	uint16_t DipSwitchPin1,
GPIO_TypeDef *DipSwitchPort2,	uint16_t DipSwitchPin2,
GPIO_TypeDef *DipSwitchPort3,	uint16_t DipSwitchPin3);

#endif /* INC_SUMOROBOT_H_ */
