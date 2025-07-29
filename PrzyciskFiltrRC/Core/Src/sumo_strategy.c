/*
 * sumo_strategy.c
 *
 *  Created on: Jul 29, 2025
 *      Author: jakub
 */
#include "sumo_strategy.h"
#include "main.h"

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

void Sumo_Init(SumoMotors_t *SumoMotors, Motor_t *MotorL, Motor_t *MotorR)
{
	SumoMotors->MotorL = MotorL;
	SumoMotors->MotorR = MotorR;
}
