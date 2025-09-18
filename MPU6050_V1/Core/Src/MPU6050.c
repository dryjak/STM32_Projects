/*
 * MPU6050.c
 *
 *  Created on: Sep 6, 2025
 *      Author: jakub
 */
#include "MPU6050.h"
#include "main.h"
#include "math.h"

typedef union{
	struct
	{
		uint8_t ByteLow;
		uint8_t ByteHigh;
	}Byte;

	int16_t Var16u;
}ConvTo16_t;


//to calculate degrees from gyro
float gx_offset = 0.0f;
float gy_offset = 0.0f;
float gz_offset = 0.0f;


uint8_t MPU6050_Init(MPU6050_t *MPU6050, I2C_HandleTypeDef *Hi2c, uint16_t Address)
{
	uint8_t CheckID;

	MPU6050->hi2c 		= 	Hi2c;
	MPU6050->address 	= 	Address;

	CheckID = Read8(MPU6050, 0x75);
	if (CheckID != 0x68)
	{
		return 1;
	}
	return 0;
}

uint8_t Read8(MPU6050_t *MPU6050, uint8_t Register)
{
	uint8_t Value;

	HAL_I2C_Mem_Read(MPU6050->hi2c, MPU6050->address, Register, 1, &Value, 1, MPU6050_TIMEOUT);

	return Value;
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

HAL_StatusTypeDef MPU6050_WHO_AM_I (MPU6050_t *MPU6050, uint8_t *Who_am_I)
{
	return Read8(MPU6050, 0x75);
}



HAL_StatusTypeDef MPU6050_MemRead(MPU6050_t *MPU6050, uint8_t Reg, uint8_t *Data, uint16_t Size)
{
	return HAL_I2C_Mem_Read(MPU6050->hi2c, (MPU6050->address << 1), Reg, I2C_MEMADD_SIZE_8BIT, Data, Size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_MemWrite(MPU6050_t *MPU6050, uint8_t Reg, uint8_t Data)
{
	return HAL_I2C_Mem_Write(MPU6050->hi2c, (MPU6050->address << 1), Reg, I2C_MEMADD_SIZE_8BIT, &Data, 1, HAL_MAX_DELAY);
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

HAL_StatusTypeDef MPU6050_ReadAcceleration(MPU6050_t *MPU6050, Accel_t *Accelerations)
{


	uint8_t AccelData[6];

	//Read Acceleration value
	if ((MPU6050_MemRead(MPU6050, ACCEL_XOUT_H, AccelData, 6)) == HAL_ERROR)
	{
		return HAL_ERROR;
	}

	/*
	//practising union
	ConvTo16_t AccelX, AccelY, AccelZ;

	AccelX.Byte.ByteHigh 	= AccelData[0];
	AccelX.Byte.ByteLow 	= AccelData[1];
	AccelY.Byte.ByteHigh 	= AccelData[2];
	AccelY.Byte.ByteLow 	= AccelData[3];
	AccelZ.Byte.ByteHigh 	= AccelData[4];
	AccelZ.Byte.ByteLow 	= AccelData[5];


	*Gx = AccelX.Var16u;
	*Gy = AccelY.Var16u;
	*Gz = AccelZ.Var16u;
	*/

	Accelerations->AccelX = ((int16_t) (AccelData[0] << 8) | (AccelData[1])) / 16384.0f;
	Accelerations->AccelY = ((int16_t) (AccelData[2] << 8) | (AccelData[3])) / 16384.0f;
	Accelerations->AccelZ = ((int16_t) (AccelData[4] << 8) | (AccelData[5])) / 16384.0f;


	return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ReadGyro(MPU6050_t *MPU6050, Gyro_t *Gyro)
{
	uint8_t GyroData[6];

	//Read Acceleration value
	if ((MPU6050_MemRead(MPU6050, GYRO_XOUT_H, GyroData, 6)) == HAL_ERROR)
	{
		return HAL_ERROR;
	}


	Gyro->GyroX = ((int16_t) (GyroData[0] << 8) | (GyroData[1])) / 131.0;
	Gyro->GyroY = ((int16_t) (GyroData[2] << 8) | (GyroData[3])) / 131.0;
	Gyro->GyroZ = ((int16_t) (GyroData[4] << 8) | (GyroData[5])) / 131.0;

	return HAL_OK;
}

HAL_StatusTypeDef MPU6050_MeanAccelerations(MPU6050_t *MPU6050, Accel_t *Accelerations, Accel_t *AccelerationsMean)
{
	uint8_t i = 0;
	uint8_t Sample = 50;
	int32_t AccelSumX, AccelSumY, AccelSumZ;

	MPU6050_ReadAcceleration(MPU6050, Accelerations);

	for(i = 0; i < Sample; i++)
	{
		AccelSumX += Accelerations->AccelX;
		AccelSumY += Accelerations->AccelY;
		AccelSumZ += Accelerations->AccelZ;
	}

	AccelerationsMean->AccelX = AccelSumX / Sample;
	AccelerationsMean->AccelY = AccelSumY / Sample;
	AccelerationsMean->AccelZ = AccelSumZ / Sample;

	return HAL_OK;

}

HAL_StatusTypeDef MPU6050_MeanGyro(MPU6050_t *MPU6050, Gyro_t *Gyro,  Gyro_t *GyroMean)
{
	uint8_t i = 0;
	uint8_t Sample = 50;
	int32_t GyroSumX, GyroSumY, GyroSumZ;

	MPU6050_ReadGyro(MPU6050, Gyro);

	for(i = 0; i < Sample; i++)
	{
		GyroSumX += Gyro->GyroX;
		GyroSumY += Gyro->GyroY;
		GyroSumZ += Gyro->GyroZ;
	}

	GyroMean->GyroX = GyroSumX / Sample;
	GyroMean->GyroY = GyroSumY / Sample;
	GyroMean->GyroZ = GyroSumZ / Sample;

	return HAL_OK;

}

HAL_StatusTypeDef MPU6050_AccelToDeg(MPU6050_t *MPU6050, Accel_t *Accelerations, float *Roll, float *Pitch)
{
	if (MPU6050_ReadAcceleration(MPU6050, Accelerations) != HAL_OK)
	{
		return HAL_ERROR;
	}

	*Pitch = atan2f(Accelerations->AccelY, Accelerations->AccelZ) * 180.0 / M_PI;
	*Roll = atan2f(-(Accelerations->AccelX), sqrtf(Accelerations->AccelY * Accelerations->AccelY + Accelerations->AccelZ * Accelerations->AccelZ)) * 180.0 / M_PI;

	return HAL_OK;
}

HAL_StatusTypeDef MPU6050_GyroToDps	(MPU6050_t *MPU6050, Gyro_t *Gyro, float *DpsX, float *DpsY, float *DpsZ)
{
	// najpierw policz średnie wartości gyro
    if (MPU6050_ReadGyro(MPU6050, Gyro) != HAL_OK)
    {
        return HAL_ERROR;
    }

    // konwersja na dps (±250°/s)
    *DpsX = ((float)Gyro->GyroX - gx_offset) / 131.0f;
    *DpsY = ((float)Gyro->GyroY - gy_offset) / 131.0f;
    *DpsZ = ((float)Gyro->GyroZ - gz_offset) / 131.0f;

	return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ComputeAngles(MPU6050_t *MPU6050, Accel_t *Accelerations, Gyro_t *Gyro, float *Roll, float *Pitch, float dt)
{
	if (MPU6050_ReadGyro(MPU6050, Gyro) != HAL_OK)
	{
		return HAL_ERROR;
	}

	if (MPU6050_ReadAcceleration(MPU6050, Accelerations) != HAL_OK)
	{
		return HAL_ERROR;
	}

	float AccelerationRoll, AccelerationPitch;
	float GyroDpsX, GyroDpsY, GyroDpsZ;
	GyroDpsX = Gyro->GyroX;
	GyroDpsY = Gyro->GyroY;
	GyroDpsZ = Gyro->GyroZ;

	//change to degrees
	//acceleration to degrees
	MPU6050_AccelToDeg(MPU6050, Accelerations,  &AccelerationRoll, &AccelerationPitch);
	//dps to degrees
	MPU6050_GyroToDps(MPU6050, Gyro, &GyroDpsX, &GyroDpsY, &GyroDpsZ);

	//calculating roll and pitch from gyro
	static float GyroRoll = 0;
	static float GyroPitch = 0;

	GyroRoll  += GyroDpsX * dt;
	GyroPitch += GyroDpsY * dt;

	//complementary filtr
	float alpha = 0.98;
	*Roll = (alpha * GyroRoll) + ((1 - alpha) * AccelerationRoll);
	*Pitch = (alpha * GyroPitch) + ((1-alpha) * AccelerationPitch);

	return HAL_OK;
}

