/*
 * MPU6050.c
 *
 *  Created on: Sep 6, 2025
 *      Author: jakub
 */
#include "MPU6050.h"
#include "main.h"



void MPU6050_WHO_AM_I (MPU6050_t *MPU6050, uint8_t Who_am_I)
{

}

void MPU6050_Init(MPU6050_t *MPU6050, I2C_HandleTypeDef *Hi2c, uint16_t Address)
{

}
void MPU6050_MemRead(MPU6050_t *MPU6050, uint8_t *Reg, uint8_t *Data, uint16_t Size)
{
	HAL_I2C_Mem_Read(MPU6050->Hi2c, (MPU6050->Address << 1), I2C_MEMADD_SIZE_8BIT, Size, Data, Size, HAL_MAX_DELAY);
}
