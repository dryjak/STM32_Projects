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
#define ADR0		 		0x68
#define ADR1 				0x69
#define PWR_MGMT_1			0x6B
#define GYRO_CONFIG 		0x1B
#define ACCEL_CONFIG 		0x1C

#define ACCEL_XOUT_H		0x3B
#define ACCEL_XOUT_L		0x3C
#define ACCEL_YOUT_H		0x3D
#define ACCEL_YOUT_L		0x3E
#define ACCEL_ZOUT_H		0x3F
#define ACCEL_ZOUT_L		0x40

#define GYRO_XOUT_H			0x43
#define GYRO_XOUT_L			0x44
#define GYRO_YOUT_H			0x45
#define GYRO_YOUT_L			0x46
#define GYRO_ZOUT_H			0x47
#define GYRO_ZOUT_L			0x48


#define MPU6050_TIMEOUT 	500

typedef struct{

	I2C_HandleTypeDef *hi2c;
	uint16_t address;

}MPU6050_t;

typedef struct
{
	float AccelX;
	float AccelY;
	float AccelZ;
}Accel_t;

typedef struct
{
	float OffsetX;
	float OffsetY;
	float OffsetZ;
}AccelOffset_t;

typedef struct
{
	int16_t GyroX;
	int16_t GyroY;
	int16_t GyroZ;
}Gyro_t;

typedef struct
{
	float OffsetX;
	float OffsetY;
	float OffsetZ;
}GyroOffset_t;

typedef enum
{
	MPU6050_OK,
	MPU6050_ERROR
}MPU6050_STATE_t;




uint8_t MPU6050_WHO_AM_I (MPU6050_t *MPU6050);
HAL_StatusTypeDef MPU6050_MemRead(MPU6050_t *MPU6050, uint8_t Reg, uint8_t *Data, uint16_t Size);
HAL_StatusTypeDef MPU6050_MemWrite(MPU6050_t *MPU6050, uint8_t Reg, uint8_t Data);
MPU6050_STATE_t MPU6050_Init(MPU6050_t *MPU6050, I2C_HandleTypeDef *Hi2c, uint16_t Address);
MPU6050_STATE_t MPU6050_WakeUp(MPU6050_t *MPU6050);
MPU6050_STATE_t MPU6050_SetAccelerationRange(MPU6050_t *MPU6050);
MPU6050_STATE_t MPU6050_SetGyroRange(MPU6050_t *MPU6050);
MPU6050_STATE_t MPU6050_ReadAcceleration(MPU6050_t *MPU6050, Accel_t *Accelerations, AccelOffset_t AccelOffset);


uint8_t Read8(MPU6050_t *MPU6050, uint8_t Register);
MPU6050_STATE_t MPU6050_CalibrateAccel(MPU6050_t *MPU6050, AccelOffset_t *AccelOffset);





#endif /* INC_MPU6050_H_ */
