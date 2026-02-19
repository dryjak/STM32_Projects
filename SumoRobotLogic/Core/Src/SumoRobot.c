/*
 * SumoRobot.c
 *
 *  Created on: Feb 19, 2026
 *      Author: dryla
 */


#include "SumoRobot.h"

#define SUMO_HIGH_STATE 1	//used for sensors when white line is detected or enemy found
#define SUMO_LOW_STATE  0

static void CheckFloorColor(SumoRobot_t *SumoRobot);


void SumoRobot_ReadTactics(SumoRobot_t *SumoRobot)
{
	if(HAL_GPIO_ReadPin(SumoRobot->Hardware.DipSwitchPort1, SumoRobot->Hardware.DipSwitchPin1))
	{
		SumoRobot->Tactics |= (1 << 0);	// XXXX XXX1
	}
	if(HAL_GPIO_ReadPin(SumoRobot->Hardware.DipSwitchPort2, SumoRobot->Hardware.DipSwitchPin2))
	{
		SumoRobot->Tactics |= (1 << 1);	// XXXX XX1X
	}
	if(HAL_GPIO_ReadPin(SumoRobot->Hardware.DipSwitchPort3, SumoRobot->Hardware.DipSwitchPin3))
	{
		SumoRobot->Tactics |= (1 << 2);	// XXXXX1XX
	}
}

void SumoRobot_UpdateSensors(SumoRobot_t *SumoRobot)
{
	//read distance sensor data
	if(HAL_GPIO_ReadPin(SumoRobot->Hardware.DistancePortM, SumoRobot->Hardware.DistancePinM) == GPIO_PIN_RESET)
		SumoRobot->Sensors.DistanceM = SUMO_HIGH_STATE;
	else
		SumoRobot->Sensors.DistanceM = SUMO_LOW_STATE;

	if(HAL_GPIO_ReadPin(SumoRobot->Hardware.DistancePortR, SumoRobot->Hardware.DistancePinR) == GPIO_PIN_RESET)
		SumoRobot->Sensors.DistanceR = SUMO_HIGH_STATE;
	else
		SumoRobot->Sensors.DistanceR = SUMO_LOW_STATE;

	if(HAL_GPIO_ReadPin(SumoRobot->Hardware.DistancePortL, SumoRobot->Hardware.DistancePinL) == GPIO_PIN_RESET)
		SumoRobot->Sensors.DistanceL = SUMO_HIGH_STATE;
	else
		SumoRobot->Sensors.DistanceL = SUMO_LOW_STATE;

	//check floor color based on ADC readings
	CheckFloorColor(SumoRobot);
}

static void CheckFloorColor(SumoRobot_t *SumoRobot)
{
	if(SumoRobot->FlorSensorAdcL > ADC_FLOR_SENSOR_BORDER)	// darker area results in higher number
	{
		SumoRobot->Sensors.FlorL = SUMO_LOW_STATE;
	}
	else
	{
		SumoRobot->Sensors.FlorL = SUMO_HIGH_STATE;
	}
	if(SumoRobot->FlorSensorAdcR > ADC_FLOR_SENSOR_BORDER)	// darker area results in higher number
	{
		SumoRobot->Sensors.FlorR = SUMO_LOW_STATE;
	}
	else
	{
		SumoRobot->Sensors.FlorR= SUMO_HIGH_STATE;
	}
}

void SumoRobot_Init(SumoRobot_t *SumoRobot,
GPIO_TypeDef *FlorPortL,	uint16_t FlorPinL,
GPIO_TypeDef *FlorPortR,	uint16_t FlorPinR,

GPIO_TypeDef *DistancePortM,	uint16_t DistancePinM,
GPIO_TypeDef *DistancePortL,	uint16_t DistancePinL,
GPIO_TypeDef *DistancePortR,	uint16_t DistancePinR,

//I have used 3 pin dip switch
GPIO_TypeDef *DipSwitchPort1,	uint16_t DipSwitchPin1,
GPIO_TypeDef *DipSwitchPort2,	uint16_t DipSwitchPin2,
GPIO_TypeDef *DipSwitchPort3,	uint16_t DipSwitchPin3)
{
	SumoRobot->Move = 0;
	SumoRobot->Tactics = 0;
	SumoRobot->Sensors.AllSensors = 0;

	SumoRobot->Hardware.FlorPortR 	= FlorPortR;
	SumoRobot->Hardware.FlorPinR 	= FlorPinR;
	SumoRobot->Hardware.FlorPortL 	= FlorPortL;
	SumoRobot->Hardware.FlorPinL 	= FlorPinL;

	SumoRobot->Hardware.DistancePortM	= DistancePortM;
	SumoRobot->Hardware.DistancePinM 	= DistancePinM;
	SumoRobot->Hardware.DistancePortM	= DistancePortR;
	SumoRobot->Hardware.DistancePinM 	= DistancePinR;
	SumoRobot->Hardware.DistancePortL	= DistancePortL;
	SumoRobot->Hardware.DistancePinL 	= DistancePinL;
}


