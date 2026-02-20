/*
 * SumoRobot.c
 *
 *  Created on: Feb 19, 2026
 *      Author: dryla
 */


#include "SumoRobot.h"

#define SUMO_HIGH_STATE 1	//used for sensors when white line is detected or enemy found
#define SUMO_LOW_STATE  0

#define DIR_RIGHT 1
#define DIR_LEFT  0

typedef enum{
	FIGHT_PHASE_OPENING = 0,
	FIGHT_PHASE_NORMAL
}FightPhase_t;

typedef enum{
	LAST_SEEN_NONE = 0,
	LAST_SEEN_MIDDLE,
	LAST_SEEN_RIGHT,
	LAST_SEEN_LEFT
}LastSeen_t;

//Static, private functions
static void CheckFloorColor(SumoRobot_t *SumoRobot);
static void SumoRobot_NormalMode(SumoRobot_t *SumoRobot);
static void SumoRobot_StartTactic(SumoRobot_t *SumoRobot);

static FightPhase_t CurrentPhase = FIGHT_PHASE_OPENING;
static uint32_t OpeningPhaseStartTime = 0;

void SumoRobot_ReadTactics(SumoRobot_t *SumoRobot)
{
	SumoRobot->Tactics = 0;
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
	//should i make it shorter?

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

void SumoRobot_Task(SumoRobot_t *SumoRobot)
{
	//Firstly check if there is a line
	if (SumoRobot->Sensors.FlorL || SumoRobot->Sensors.FlorR)
	{
		CurrentPhase = FIGHT_PHASE_NORMAL;	//if we see the line we don't make starting sequence
		OpeningPhaseStartTime = 0; //reset tactic timer

		if (SumoRobot->Sensors.FlorL && SumoRobot->Sensors.FlorR) {
			SumoRobot->Move = MOVE_BACKWARD; 	// Line up front -> go backward
		} else if (SumoRobot->Sensors.FlorL) {
			SumoRobot->Move = MOVE_TURN_RIGHT;	// Line from left -> go right
		} else {
			SumoRobot->Move = MOVE_TURN_LEFT;  	// Line from right -> go left
		}

		return;
	}

	//fight stage
	switch (CurrentPhase)
	{
	case FIGHT_PHASE_OPENING:	//Fight phase opening
		SumoRobot_StartTactic(SumoRobot);
		break;
	case FIGHT_PHASE_NORMAL:	//Fight phase normal
		SumoRobot_NormalMode(SumoRobot);
		break;
	}

}

void SumoRobot_StartTactic(SumoRobot_t *SumoRobot)
{
	if (OpeningPhaseStartTime == 0)
	{
		OpeningPhaseStartTime = HAL_GetTick();
	}
	uint32_t Elapsed = HAL_GetTick() - OpeningPhaseStartTime;

	switch (SumoRobot->Tactics)
	{
	case 0:	// 000 - normal mode
		CurrentPhase = FIGHT_PHASE_NORMAL;
		break;
	case 1:	// 001 - fast rotate left until middle sensor sees enemy
			SumoRobot->Move = MOVE_TURN_LEFT;
			// Stop rotating if we see enemy or time has passed
			if (SumoRobot->Sensors.DistanceM == SUMO_HIGH_STATE || Elapsed > 500) {
				CurrentPhase = FIGHT_PHASE_NORMAL;
			}
		break;
	case 2:	// 010 - fast rotate right until middle sensor sees enemy
			SumoRobot->Move = MOVE_TURN_RIGHT;
			// Stop rotating if we see enemy or time has passed
			if (SumoRobot->Sensors.DistanceM == SUMO_HIGH_STATE || Elapsed > 500) {
				CurrentPhase = FIGHT_PHASE_NORMAL;
			}
		break;
	case 3: // 011 - go with curve left
		SumoRobot->Move = MOVE_TURN_SLIGHT_LEFT;
		// Stop rotating if we see enemy or time has passed
		if (Elapsed > 350)
			CurrentPhase = FIGHT_PHASE_NORMAL;
		break;
	case 4:	// 100 - go with curve right
		SumoRobot->Move = MOVE_TURN_SLIGHT_RIGHT;
		// Stop rotating if we see enemy or time has passed
		if (Elapsed > 350)
			CurrentPhase = FIGHT_PHASE_NORMAL;
		break;
	case 5: // 101 - go back and wait
		SumoRobot->Move = MOVE_SLIGHT_BACKWARD;
		// Stop moving if time has passed
		if (Elapsed > 350)
			CurrentPhase = FIGHT_PHASE_NORMAL;
		break;
	case 7: // 111 - go straight for few seconds
		SumoRobot->Move = MOVE_FORWARD;
		// Stop moving if time has passed
		if (Elapsed > 500) {
			CurrentPhase = FIGHT_PHASE_NORMAL;
		}
		break;

	default:
		CurrentPhase = FIGHT_PHASE_NORMAL;
		break;
	}
}

void SumoRobot_NormalMode(SumoRobot_t *SumoRobot)
{
	static LastSeen_t LastSeen = LAST_SEEN_NONE;

	//check distance sensors
	if(SumoRobot->Hardware.DistancePinM)
	{
		//go straight
		SumoRobot->Move = MOVE_FORWARD;
		LastSeen = LAST_SEEN_MIDDLE;
	}
	else if(SumoRobot->Hardware.DistancePinL)
	{
		//go left
		SumoRobot->Move = MOVE_TURN_LEFT;
		LastSeen = LAST_SEEN_LEFT;
	}
	else if(SumoRobot->Hardware.DistancePinR)
	{
		//go right
		SumoRobot->Move = MOVE_TURN_RIGHT;
		LastSeen = LAST_SEEN_RIGHT;
	}
	else if(LastSeen != LAST_SEEN_NONE && LastSeen != LAST_SEEN_MIDDLE)
	{
		if(LastSeen == LAST_SEEN_LEFT)
		{
			SumoRobot->Move = MOVE_TURN_LEFT;
		}
		else if(LastSeen == LAST_SEEN_RIGHT)
		{
			SumoRobot->Move = MOVE_TURN_RIGHT;
		}
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
	SumoRobot->Hardware.DistancePortR	= DistancePortR;
	SumoRobot->Hardware.DistancePinR 	= DistancePinR;
	SumoRobot->Hardware.DistancePortL	= DistancePortL;
	SumoRobot->Hardware.DistancePinL 	= DistancePinL;
}


