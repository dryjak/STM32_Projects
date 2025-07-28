/*
 * motor_simple.h
 *
 *  Created on: Jul 26, 2025
 *      Author: jakub
 */

#ifndef INC_MOTOR_SIMPLE_H_
#define INC_MOTOR_SIMPLE_H_

typedef enum{
	MOTOR_OK = 0,
	MOTOR_ERROR
}MotorStatus_t;

typedef struct
{
	TIM_HandleTypeDef	*htim;
	uint32_t 			Channel;

	GPIO_TypeDef 		*MotorDir1Port;
	uint16_t			MotorDir1Pin;

	GPIO_TypeDef 		*MotorDir2Port;
	uint16_t			MotorDir2Pin;

	uint8_t 			MotorPWM;
	uint8_t				Direction;

}Motor_t;

MotorStatus_t Motor_Init(Motor_t *Motor, TIM_HandleTypeDef *Timer, uint32_t TimerChannel, uint16_t PWM, GPIO_TypeDef *Dir1Port, uint16_t Dir1Pin, GPIO_TypeDef *Dir2Port, uint16_t Dir2Pin);


#endif /* INC_MOTOR_SIMPLE_H_ */
