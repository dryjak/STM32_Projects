/*
 * sumo_strategy.h
 *
 *  Created on: Jul 29, 2025
 *      Author: jakub
 */

#ifndef INC_SUMO_STRATEGY_H_
#define INC_SUMO_STRATEGY_H_

#include "motor_simple.h"

//struktura / coś do zapamiętania strony wykrycia



typedef enum{
	STATE_ATTACK = 0,
	STATE_SEARCH,
	STATE_TURN_LEFT,
	STATE_TURN_RIGHT
}SumoState_t;

typedef struct{
	Motor_t *MotorL;
	Motor_t *MotorR;
	//for simulation of sensors
	GPIO_TypeDef *SensorLPort;
	uint16_t SensorLPin;
	GPIO_TypeDef *SensorRPort;
	uint16_t SensorRPin;
	GPIO_TypeDef *SensorMPort;
	uint16_t SensorMPin;
}SumoMotors_t;

typedef struct{
	uint16_t *DistanceSensorL;
	float DistanceSensorLVoltage;
	float DistanceSensorLVoltageFiltered;

	uint16_t *DistanceSensorR;
	float DistanceSensorRVoltage;
	float DistanceSensorRVoltageFiltered;

	uint16_t FlorSensorL;
	uint16_t FlorSensorR;
}SumoSensors_t;


void Sumo_InitMotors(SumoMotors_t *SumoMotors, Motor_t *MotorL, Motor_t *MotorR);
void Sumo_InitDistanceSensors(SumoSensors_t *SumoSensors, uint16_t *DistanceSensorL, uint16_t *DistanceSensorR);

void SumoAtack(SumoMotors_t *SumoMotors);
void Sumo_SearchRight(SumoMotors_t *SumoMotors);
void Sumo_SearchLeft(SumoMotors_t *SumoMotors);
void Sumo_TurnSlightRight(SumoMotors_t *SumoMotors);
void Sumo_TurnSlightLeft(SumoMotors_t *SumoMotors);
void Sumo_Stop(SumoMotors_t *SumoMotors);

//Sensors
void Sumo_UpdateSensors(SumoSensors_t *SumoSensors);
void AdcToVoltage(uint16_t *AdcValue, float *Voltage);
void IIRFilter(float alpha, float input, float *filtered_value);

void Sumo_UpdateState(SumoState_t *CurrentState, SumoMotors_t *SumoMotors, SumoSensors_t *SumoSensors);


#endif /* INC_SUMO_STRATEGY_H_ */
