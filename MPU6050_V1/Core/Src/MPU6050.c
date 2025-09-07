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

}
HAL_StatusTypeDef MPU6050_MemRead(MPU6050_t *MPU6050, uint8_t Reg, uint8_t *Data, uint16_t Size)
{
	return HAL_I2C_Mem_Read(MPU6050->hi2c, (MPU6050->address << 1), I2C_MEMADD_SIZE_8BIT, Size, Data, Size, HAL_MAX_DELAY);
}
