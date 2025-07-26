/*
 * motor_simple.h
 *
 *  Created on: Jul 26, 2025
 *      Author: jakub
 */

#ifndef INC_MOTOR_SIMPLE_H_
#define INC_MOTOR_SIMPLE_H_

typedef enum{
	MOTORS_OK = 0,
	MOTORS_ERROR
}MotorsStatus_t;

typedef struct
{
	TIM_HandleTypeDef	*htim;

	GPIO_TypeDef 		*MotorLeftPort;
	uint16_t			MotorLeftPin;

	GPIO_TypeDef 		*MotorRightPort;
	uint16_t			MotorRightPin;

}Motors_t;

MotorsStatus_t Motors_Init(Motors_t Motor, TIM_HandleTypeDef *timer, GPIO_TypeDef *MotorLeftPort, uint16_t MotorLeftPin, GPIO_TypeDef *MotorRightPort, uint16_t MotorRightPin);


#endif /* INC_MOTOR_SIMPLE_H_ */
