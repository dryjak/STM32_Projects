/*
 * MPU6050.c
 *
 *  Created on: Sep 6, 2025
 *      Author: jakub
 */
#include "MPU6050.h"
#include "main.h"

typedef union{
	struct
	{
		uint8_t ByteLow;
		uint8_t ByteHigh;
	}Byte;

	uint16_t Var16u;
}ConvTo16_t;


HAL_StatusTypeDef MPU6050_WHO_AM_I (MPU6050_t *MPU6050, uint8_t *Who_am_I)
{
	return MPU6050_MemRead(MPU6050, WHO_AM_I, Who_am_I, 1);
}

void MPU6050_Init(MPU6050_t *MPU6050, I2C_HandleTypeDef *Hi2c, uint16_t Address)
{
	MPU6050->hi2c 		= 	Hi2c;
	MPU6050->address 	= 	Address;
}
HAL_StatusTypeDef MPU6050_MemRead(MPU6050_t *MPU6050, uint8_t Reg, uint8_t *Data, uint16_t Size)
{
	return HAL_I2C_Mem_Read(MPU6050->hi2c, (MPU6050->address << 1), Reg, I2C_MEMADD_SIZE_8BIT, Data, Size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_MemWrite(MPU6050_t *MPU6050, uint8_t Reg, uint8_t Data)
{
	return HAL_I2C_Mem_Write(MPU6050->hi2c, (MPU6050->address << 1), Reg, I2C_MEMADD_SIZE_8BIT, &Data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_WakeUp(MPU6050_t *MPU6050)
{
	//Firstly you need to read the register value
	uint8_t Register;
	if ((MPU6050_MemRead(MPU6050, MPU6050_PWR_MGMT_1, &Register, 1)) != HAL_OK)
	{
		return HAL_ERROR;
	}

	//Setting 6th bit to 0 to disable sleep mode
	Register &= ~(1 << 6);		//wyzeruj bit 6
	return MPU6050_MemWrite(MPU6050, MPU6050_PWR_MGMT_1, Register);
}

HAL_StatusTypeDef MPU6050_SetGyroRange(MPU6050_t *MPU6050)
{
	//Firstly you need to read the register value
	uint8_t Register;
	if ((MPU6050_MemRead(MPU6050, GYRO_CONFIG, &Register, 1)) != HAL_OK)
	{
		return HAL_ERROR;
	}

	//setting FS_SEL[1:0] in order to setfull scale range
	Register &= (~((1 << 4) | (1 << 5)));		//setting 250 stopni na sekunde
	return MPU6050_MemWrite(MPU6050, GYRO_CONFIG, Register);
}

HAL_StatusTypeDef MPPU6050_SetAcceleration(MPU6050_t *MPU6050)
{
	//Firstly you need to read the register value
	uint8_t Register;
	if ((MPU6050_MemRead(MPU6050, ACCEL_CONFIG, &Register, 1)) != HAL_OK)
	{
		return HAL_ERROR;
	}

	Register &= (~((1 << 4)|(1 << 5)));		//setting acceleration to -/+ 2g
	return MPU6050_MemWrite(MPU6050, ACCEL_CONFIG, Register);
}

HAL_StatusTypeDef MPU6050_ReadAcceleration(MPU6050_t *MPU6050, uint16_t *Gx, uint16_t *Gy, uint16_t *Gz)
{
	ConvTo16_t AccelX, AccelY, AccelZ;
	uint8_t ByteLow, ByteHigh;

	uint8_t AccelData[6];

	//Read Acceleration value
	if ((MPU6050_MemRead(MPU6050, ACCEL_XOUT_L, AccelData, 6)) == HAL_ERROR)
	{
		return HAL_ERROR;
	}

	AccelX.Byte.ByteHigh 	= AccelData[0];
	AccelX.Byte.ByteLow 	= AccelData[1];
	AccelY.Byte.ByteHigh 	= AccelData[2];
	AccelY.Byte.ByteLow 	= AccelData[3];
	AccelZ.Byte.ByteHigh 	= AccelData[4];
	AccelZ.Byte.ByteLow 	= AccelData[5];

	*Gx = AccelX.Var16u;
	*Gy = AccelY.Var16u;
	*Gz = AccelZ.Var16u;

	return HAL_OK;
}
