/*
 * sumo_strategy.c
 *
 *  Created on: Jul 29, 2025
 *      Author: jakub
 */
#include "sumo_strategy.h"
#include "main.h"


typedef enum{
	STATE_ATTACK = 0,
	STATE_SEARCH,
	STATE_TURN_LEFT,
	STATE_TURN_RIGHT
}SumoState;

SumoState CurrentState = STATE_SEARCH;

void SumoAtack(SumoMotors_t *SumoMotors)
{
	Motor_SetRideParameters(SumoMotors->MotorL, 100, 1);  // Przód
	Motor_SetRideParameters(SumoMotors->MotorR, 100, 1);
	Motor_Ride(SumoMotors->MotorL);
	Motor_Ride(SumoMotors->MotorR);
}

void Sumo_SearchRight(SumoMotors_t *SumoMotors)
{
	Motor_SetRideParameters(SumoMotors->MotorL, 60, 1);   // Lewy do przodu
	Motor_SetRideParameters(SumoMotors->MotorR, 60, 0);   // Prawy do tyłu
	Motor_Ride(SumoMotors->MotorL);
	Motor_Ride(SumoMotors->MotorR);
}
void Sumo_SearchLeft(SumoMotors_t *SumoMotors)
{
	Motor_SetRideParameters(SumoMotors->MotorL, 60, 0);   // Lewy do przodu
	Motor_SetRideParameters(SumoMotors->MotorR, 60, 1);   // Prawy do tyłu
	Motor_Ride(SumoMotors->MotorL);
	Motor_Ride(SumoMotors->MotorR);
}
void Sumo_TurnSlightRight(SumoMotors_t *SumoMotors)
{
	Motor_SetRideParameters(SumoMotors->MotorL, 70, 1);   // Lewy do przodu
	Motor_SetRideParameters(SumoMotors->MotorR, 40, 0);   // Prawy do tyłu
	Motor_Ride(SumoMotors->MotorL);
	Motor_Ride(SumoMotors->MotorR);
}
void Sumo_TurnSlightLeft(SumoMotors_t *SumoMotors)
{
	Motor_SetRideParameters(SumoMotors->MotorL, 40, 0);   // Lewy do przodu
	Motor_SetRideParameters(SumoMotors->MotorR, 70, 1);   // Prawy do tyłu
	Motor_Ride(SumoMotors->MotorL);
	Motor_Ride(SumoMotors->MotorR);
}
void Sumo_Stop(SumoMotors_t *SumoMotors)
{
	Motor_SetRideParameters(SumoMotors->MotorL, 0, 0);   // Lewy do przodu
	Motor_SetRideParameters(SumoMotors->MotorR, 0, 1);   // Prawy do tyłu
	Motor_Ride(SumoMotors->MotorL);
	Motor_Ride(SumoMotors->MotorR);
}

//void UpdateState(SumoState )


void Sumo_Init(SumoMotors_t *SumoMotors, Motor_t *MotorL, Motor_t *MotorR)
{
	SumoMotors->MotorL = MotorL;
	SumoMotors->MotorR = MotorR;
}
