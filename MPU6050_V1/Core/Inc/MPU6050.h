/*
 * MPU6050.h
 *
 *  Created on: Sep 6, 2025
 *      Author: jakub
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "main.h"

#define WHO_AM_I 0x75
#define MPU6050_ADR0 0x68
#define MPU6050_ADR1 0x69

typedef struct{

	I2C_HandleTypeDef *hi2c;
	uint16_t address;

}MPU6050_t;

HAL_StatusTypeDef MPU6050_WHO_AM_I (MPU6050_t *MPU6050, uint8_t *Who_am_I);
HAL_StatusTypeDef MPU6050_MemRead(MPU6050_t *MPU6050, uint8_t Reg, uint8_t *Data, uint16_t Size);


#endif /* INC_MPU6050_H_ */
