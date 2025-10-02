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



typedef struct
{
	float AccelX;
	float AccelY;
	float AccelZ;
}Accel_t;

typedef struct
{
	int16_t AccelX;
	int16_t AccelY;
	int16_t AccelZ;
}AccelRaw_t;

typedef struct
{
	float OffsetX;
	float OffsetY;
	float OffsetZ;
}AccelOffset_t;

typedef struct
{
	float GyroX;
	float GyroY;
	float GyroZ;
}Gyro_t;

typedef struct
{
	int16_t GyroX;
	int16_t GyroY;
	int16_t GyroZ;
}GyroRaw_t;

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

typedef struct
{
	float Roll;
	float Pitch;
	float Yaw;
}FirstMeasure_t;

typedef struct{

	I2C_HandleTypeDef *hi2c;
	uint16_t address;
	AccelOffset_t AccelOffset;
	GyroOffset_t GyroOffset;

	FirstMeasure_t FirstMeasure;

}MPU6050_t;


MPU6050_STATE_t MPU6050_Init(MPU6050_t *MPU6050, I2C_HandleTypeDef *Hi2c, uint16_t Address);
MPU6050_STATE_t MPU6050_WakeUp(MPU6050_t *MPU6050);
uint8_t MPU6050_WHO_AM_I (MPU6050_t *MPU6050);

//functions for accel
MPU6050_STATE_t MPU6050_DegFromAccel(MPU6050_t *MPU6050, float *Roll, float *Pitch);

//functions for gyro
MPU6050_STATE_t MPU6050_DegFromGyro(MPU6050_t *MPU6050, float *RollG, float *PitchG, float *YawG);

//Function to read final angle values
MPU6050_STATE_t MPU6050_Angle(MPU6050_t *MPU6050, float *Roll, float *Pitch, float *Yaw);


#endif /* INC_MPU6050_H_ */
