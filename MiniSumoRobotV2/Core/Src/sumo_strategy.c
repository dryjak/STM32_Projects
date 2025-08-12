/*
 * sumo_strategy.c
 *
 *  Created on: Jul 29, 2025
 *      Author: jakub
 */
#include "sumo_strategy.h"
#include "main.h"

SumoState CurrentState = STATE_SEARCH;

uint16_t DistanceFilteredValueL;
uint16_t DistanceFilteredValueR;
float alpha = 0.3;

float VoltageL = 0.0f;
float VoltageR = 0.0f;
float FilteredVoltageL = 0.0f;
float FilteredVoltageR = 0.0f;

void AdcToVoltage(uint16_t *AdcValue, float *Voltage)
{
    *Voltage = ((*AdcValue) / 4095.0f) * 3.3f;
}

void IIRFilter(float alpha, float input, float *filtered_value)
{
    *filtered_value = alpha * (input) + (1.0f - alpha) * (*filtered_value);
}

void Sumo_UpdateSensors(SumoSensors_t *SumoSensors)
{
    // Przelicz na napięcie
	AdcToVoltage(&SumoSensors->DistanceSensorL, &VoltageL);
	AdcToVoltage(&SumoSensors->DistanceSensorR, &VoltageR);


    // Zastosuj filtr IIR
    IIRFilter(alpha, VoltageL, &FilteredVoltageL);
    IIRFilter(alpha, VoltageR, &FilteredVoltageR);
}

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

//void UpdateState(SumoState CurrentState, SumoMotors_t *SumoMotors, SumoSensors_t SumoSensors)
//{
//	if (SumoSensors->DistanceSensorL)
//	{
//		CurrentState = STATE_TURN_LEFT;
//	}
	//do zrobienia juz z wykorzystaniem sensorów więc najpierw trzeba poczekać na przesylkę i nauczyć się adc
//}



void Sumo_InitMotors(SumoMotors_t *SumoMotors, Motor_t *MotorL, Motor_t *MotorR)
{
	SumoMotors->MotorL = MotorL;
	SumoMotors->MotorR = MotorR;
}

void Sumo_InitDistanceSensors(SumoSensors_t *SumoSensors, uint16_t *DistanceSensorL, uint16_t *DistanceSensorR)
{
	SumoSensors->DistanceSensorL = DistanceSensorL;
	SumoSensors->DistanceSensorR = DistanceSensorR;
}

