/*
 * MPU6050.h
 *
 *  Created on: Sep 6, 2025
 *      Author: jakub
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#define WHO_AM_I 0x75;
#define MPU6050_ADR0 0x68
#define MPU6050_ADR1 0x69

typedef struct{
	I2C_HandleTypeDef hi2c;
}MPU6050_t;

#endif /* INC_MPU6050_H_ */
