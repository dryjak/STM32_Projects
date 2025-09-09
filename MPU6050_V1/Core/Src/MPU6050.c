/*
 * MPU6050.c
 *
 *  Created on: Sep 6, 2025
 *      Author: jakub
 */
#include "MPU6050.h"
#include "main.h"



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
	return HAL_I2C_Mem_Read(MPU6050->hi2c, (MPU6050->address << 1), I2C_MEMADD_SIZE_8BIT, Size, Data, Size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_MemWrite(MPU6050_t *MPU6050, uint8_t Reg, uint8_t Data)
{
	return HAL_I2C_Mem_Write(MPU6050->hi2c, (MPU6050->address << 1), Reg, I2C_MEMADD_SIZE_8BIT, &Data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_WakeUp(MPU6050_t *MPU6050)
{
	//setting 6 bit (sleep_ to 0 to enable the device
	uint8_t EnableDevice = MPU6050_PWR_MGMT_1 & (~(1 << 1)); 	//setting 6 bit of the register to 0
	return MPU6050_MemWrite(MPU6050, MPU6050_PWR_MGMT_1, EnableDevice); //
}
