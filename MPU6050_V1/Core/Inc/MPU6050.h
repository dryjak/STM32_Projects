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
#define GYRO_CONFIG 		0x1B
#define ACCEL_CONFIG 		0x1C

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

typedef struct
{
	int16_t AccelX;
	int16_t AccelY;
	int16_t AccelZ;
}Accel_t;

typedef struct
{
	int16_t GyroX;
	int16_t GyroY;
	int16_t GyroZ;
}Gyro_t;


HAL_StatusTypeDef MPU6050_WHO_AM_I (MPU6050_t *MPU6050, uint8_t *Who_am_I);
HAL_StatusTypeDef MPU6050_MemRead(MPU6050_t *MPU6050, uint8_t Reg, uint8_t *Data, uint16_t Size);
HAL_StatusTypeDef MPU6050_MemWrite(MPU6050_t *MPU6050, uint8_t Reg, uint8_t Data);
void MPU6050_Init(MPU6050_t *MPU6050, I2C_HandleTypeDef *Hi2c, uint16_t Address);
HAL_StatusTypeDef MPU6050_WakeUp(MPU6050_t *MPU6050);
HAL_StatusTypeDef MPPU6050_SetAcceleration(MPU6050_t *MPU6050);
HAL_StatusTypeDef MPU6050_ReadAcceleration(MPU6050_t *MPU6050, Accel_t *Accelerations);





#endif /* INC_MPU6050_H_ */
