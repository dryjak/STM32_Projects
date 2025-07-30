/*
 * sumo_strategy.h
 *
 *  Created on: Jul 29, 2025
 *      Author: jakub
 */

#ifndef INC_SUMO_STRATEGY_H_
#define INC_SUMO_STRATEGY_H_

#include "motor_simple.h"

typedef struct{
	Motor_t *MotorL;
	Motor_t *MotorR;
}SumoMotors_t;

void Sumo_Init(SumoMotors_t *SumoMotors, Motor_t *MotorL, Motor_t *MotorR);
void SumoAtack(SumoMotors_t *SumoMotors);
void Sumo_SearchRight(SumoMotors_t *SumoMotors);
void Sumo_SearchLeft(SumoMotors_t *SumoMotors);
void Sumo_TurnSlightRight(SumoMotors_t *SumoMotors);
void Sumo_TurnSlightLeft(SumoMotors_t *SumoMotors);
void Sumo_Stop(SumoMotors_t *SumoMotors);





#endif /* INC_SUMO_STRATEGY_H_ */
