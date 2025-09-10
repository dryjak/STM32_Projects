/*
 * MPU6050.h
 *
 *  Created on: Sep 6, 2025
 *      Author: jakub
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "main.h"

#define WHO_AM_I 			0x75
#define MPU6050_ADR0 		0x68
#define MPU6050_ADR1 		0x69
#define MPU6050_PWR_MGMT_1	0x6B
#define GYRO_CONFIG 		0x27
#define ACCEL_CONFIG 		0x28

#define ACCEL_XOUT_H		0x3B
#define ACCEL_XOUT_L		0x3C
#define ACCEL_YOUT_H		0x3D
#define ACCEL_YOUT_L		0x3E
#define ACCEL_ZOUT_H		0x3F
#define ACCEL_ZOUT_L		0x40

#define GYRO_XOUT_H			0x43

typedef struct{

	I2C_HandleTypeDef *hi2c;
	uint16_t address;

}MPU6050_t;

HAL_StatusTypeDef MPU6050_WHO_AM_I (MPU6050_t *MPU6050, uint8_t *Who_am_I);
HAL_StatusTypeDef MPU6050_MemRead(MPU6050_t *MPU6050, uint8_t Reg, uint8_t *Data, uint16_t Size);
void MPU6050_Init(MPU6050_t *MPU6050, I2C_HandleTypeDef *Hi2c, uint16_t Address);


#endif /* INC_MPU6050_H_ */
