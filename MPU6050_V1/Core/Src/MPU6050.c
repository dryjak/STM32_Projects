/*
 * MPU6050.c
 *
 *  Created on: Sep 6, 2025
 *      Author: jakub
 */
#include "MPU6050.h"
#include "main.h"
#include "math.h"



#define ACCEL_SCALE_FACTOR 16384.0f		//scale factor for acceleration range +/- 2g
#define GYRO_SCALE_FACTOR 131.0f		//scale factor for gyro range +/- 250 deg/s



//TO DO: odczyt z accelerometru
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


MPU6050_STATE_t MPU6050_Init(MPU6050_t *MPU6050, I2C_HandleTypeDef *Hi2c, uint16_t Address)
{
	uint8_t CheckID;

	MPU6050->hi2c 		= 	Hi2c;
	MPU6050->address 	= 	Address;

	CheckID = Read8(MPU6050, 0x75);
	if (CheckID != 0x68)
	{
		return MPU6050_ERROR;
	}

	if (MPU6050_WakeUp(MPU6050) != MPU6050_OK)
	{
		return MPU6050_ERROR;
	}
	if (MPU6050_SetAccelerationRange(MPU6050) != MPU6050_OK)
	{
		return MPU6050_ERROR;
	}
	if (MPU6050_SetGyroRange(MPU6050) != MPU6050_OK)
	{
		return MPU6050_ERROR;
	}

	MPU6050_CalibrateAccel(MPU6050, &MPU6050->AccelOffset);
	MPU6050_CalibrateGyro(MPU6050, &MPU6050->GyroOffset);

	return MPU6050_OK;
}

uint8_t Read8(MPU6050_t *MPU6050, uint8_t Register)
{
	uint8_t Value;

	HAL_I2C_Mem_Read(MPU6050->hi2c, ((MPU6050->address) << 1), Register, 1, &Value, 1, MPU6050_TIMEOUT);

	return Value;
}

MPU6050_STATE_t Write8(MPU6050_t *MPU6050, uint8_t Register, uint8_t Value)
{
	return HAL_I2C_Mem_Write(MPU6050->hi2c, (MPU6050->address) << 1, Register, 1, &Value, 1, MPU6050_TIMEOUT);
}

int16_t Read16(MPU6050_t *MPU6050, uint8_t Register)
{
	uint8_t Value[2];

	HAL_I2C_Mem_Read(MPU6050->hi2c, ((MPU6050->address) << 1), Register, 1, Value, 2, MPU6050_TIMEOUT);

	return ((Value[0] << 8) | (Value[1]));
}

uint8_t MPU6050_WHO_AM_I (MPU6050_t *MPU6050)
{
	uint8_t DeviceAdress = Read8(MPU6050, 0x75);
	return DeviceAdress;
}

MPU6050_STATE_t MPU6050_WakeUp(MPU6050_t *MPU6050)
{
	//Firstly you need to read the register value
	uint8_t Value = Read8(MPU6050, PWR_MGMT_1);

	//Disable sleep mode by setting 6th bit to 0
	Value &= ~(1 << 6);
	//Setting cycle bit to 0
	Value &= ~(1 << 5);
	//Disabling the temperature sensor
	Value |= (1 << 3);

	return Write8(MPU6050, PWR_MGMT_1, Value);
}

MPU6050_STATE_t MPU6050_SetGyroRange(MPU6050_t *MPU6050)
{
	//Firstly you need to read the register value
	uint8_t RegisterValue;
	RegisterValue = Read8(MPU6050, GYRO_CONFIG);


	//setting FS_SEL[1:0] in order to setfull scale range
	RegisterValue &= (~((1 << 4) | (1 << 5)));		//setting 250 degrees per second
	return Write8(MPU6050, GYRO_CONFIG, RegisterValue);
}

MPU6050_STATE_t MPU6050_SetAccelerationRange(MPU6050_t *MPU6050)
{
	//Firstly you need to read the register value
	uint8_t RegisterValue;
	RegisterValue = Read8(MPU6050, ACCEL_CONFIG);

	RegisterValue &= (~((1 << 4)|(1 << 5)));		//setting acceleration to -/+ 2g
	return Write8(MPU6050, ACCEL_CONFIG, RegisterValue);
}


//ACCELEROMETR DATA
MPU6050_STATE_t MPU6050_ReadAccelerationRaw(MPU6050_t *MPU6050, AccelRaw_t *AccelRaw)
{
	AccelRaw->AccelX = Read16(MPU6050, ACCEL_XOUT_H);
	AccelRaw->AccelY = Read16(MPU6050, ACCEL_YOUT_H);
	AccelRaw->AccelZ = Read16(MPU6050, ACCEL_ZOUT_H);

	return MPU6050_OK;
}
MPU6050_STATE_t MPU6050_ReadAcceleration(MPU6050_t *MPU6050, Accel_t *Accelerations, AccelOffset_t AccelOffset)
{
	AccelRaw_t AccelRaw;
	MPU6050_ReadAccelerationRaw(MPU6050, &AccelRaw);

	Accelerations->AccelX = (AccelRaw.AccelX / ACCEL_SCALE_FACTOR) - AccelOffset.OffsetX;
	Accelerations->AccelY = (AccelRaw.AccelY / ACCEL_SCALE_FACTOR) - AccelOffset.OffsetY;
	Accelerations->AccelZ = (AccelRaw.AccelZ / ACCEL_SCALE_FACTOR) - AccelOffset.OffsetZ - 1;


	return MPU6050_OK;

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
}

MPU6050_STATE_t MPU6050_CalibrateAccel(MPU6050_t *MPU6050, AccelOffset_t *AccelOffset)
{
	Accel_t Accelerations;
	int32_t SumX = 0;
	int32_t SumY = 0;
	int32_t SumZ = 0;

	uint8_t SamplesNumber = 100;
	uint8_t i = 0;

	for(i = 0; i < SamplesNumber; i++)
	{
		MPU6050_ReadAcceleration(MPU6050, &Accelerations, *AccelOffset);

		SumX += Accelerations.AccelX * ACCEL_SCALE_FACTOR;
		SumY += Accelerations.AccelY * ACCEL_SCALE_FACTOR;
		SumZ += Accelerations.AccelZ * ACCEL_SCALE_FACTOR;

		HAL_Delay(2);
	}
	AccelOffset->OffsetX = SumX / (SamplesNumber * ACCEL_SCALE_FACTOR);
	AccelOffset->OffsetY = SumY / (SamplesNumber * ACCEL_SCALE_FACTOR);
	AccelOffset->OffsetZ = (SumZ / (SamplesNumber * ACCEL_SCALE_FACTOR)) - 1;

	return MPU6050_OK;
}

MPU6050_STATE_t MPU6050_DegFromAccel(MPU6050_t *MPU6050, float *Roll, float *Pitch)
{
	Accel_t Accelerations;
	AccelOffset_t AccelOffset;

	MPU6050_CalibrateAccel(MPU6050, &AccelOffset);
	MPU6050_ReadAcceleration(MPU6050, &Accelerations, AccelOffset);

	float Ax = Accelerations.AccelX;
	float Ay = Accelerations.AccelY;
	float Az = Accelerations.AccelZ;

	*Roll  = atan2f(Ay, Az) * 180.0f / M_PI;
	*Pitch = atan2f(-Ax, sqrtf(Ay*Ay + Az*Az)) * 180.0f / M_PI;

	return MPU6050_OK;
}

//GYRO DATA
MPU6050_STATE_t MPU6050_ReadGyroRaw(MPU6050_t *MPU6050, GyroRaw_t *GyroRaw)
{
	GyroRaw->GyroX = Read16(MPU6050, GYRO_XOUT_H);
	GyroRaw->GyroY = Read16(MPU6050, GYRO_YOUT_H);
	GyroRaw->GyroZ = Read16(MPU6050, GYRO_ZOUT_H);

	return MPU6050_OK;
}

MPU6050_STATE_t MPU6050_ReadGyro(MPU6050_t *MPU6050, Gyro_t *GyroCalculated, GyroOffset_t GyroOffset)
{
	GyroRaw_t GyroRaw;
	MPU6050_ReadGyroRaw(MPU6050, &GyroRaw);

	GyroCalculated->GyroX = (GyroRaw.GyroX / GYRO_SCALE_FACTOR) - GyroOffset.OffsetX;
	GyroCalculated->GyroY = (GyroRaw.GyroY / GYRO_SCALE_FACTOR) - GyroOffset.OffsetY;
	GyroCalculated->GyroZ = (GyroRaw.GyroZ / GYRO_SCALE_FACTOR) - GyroOffset.OffsetZ;

	return MPU6050_OK;
}

MPU6050_STATE_t MPU6050_CalibrateGyro(MPU6050_t *MPU6050, GyroOffset_t *GyroOffset)
{
	GyroRaw_t Gyro;
	int32_t SumX = 0;
	int32_t SumY = 0;
	int32_t SumZ = 0;

	uint8_t SamplesNumber = 100;
	uint8_t i = 0;

	for(i = 0; i < SamplesNumber; i++)
	{
		MPU6050_ReadGyroRaw(MPU6050, &Gyro);

		SumX += Gyro.GyroX;
		SumY += Gyro.GyroY;
		SumZ += Gyro.GyroZ;

		HAL_Delay(2);
	}

	GyroOffset->OffsetX = SumX / (SamplesNumber * GYRO_SCALE_FACTOR);
	GyroOffset->OffsetY = SumY / (SamplesNumber * GYRO_SCALE_FACTOR);
	GyroOffset->OffsetZ = SumZ / (SamplesNumber * GYRO_SCALE_FACTOR);

	return MPU6050_OK;
}

MPU6050_STATE_t MPU6050_DegFromGyro(Gyro_t *Gyro, float *Roll, float *Pitch, float *Yaw, float dt)
{
	*Roll += Gyro->GyroX * dt;
	*Pitch += Gyro->GyroY * dt;
	*Yaw += Gyro->GyroZ * dt;

	return MPU6050_OK;
}

MPU6050_STATE_t MPU6050_GyroAngle(MPU6050_t *MPU6050, float *Roll, float *Pitch, float *Yaw, float dt)
{
	Accel_t Accelerations;
	Gyro_t Gyro;

	static float GyroRoll 	= 0.0f;
	static float GyroPitch 	= 0.0f;
	static float GyroYaw 	= 0.0f;

	float AccelRoll 	= 0;
	float AccelPitch 	= 0;

	//Read gyro data
	Gyro_t GyroCalculated;
	MPU6050_ReadGyro(MPU6050, &GyroCalculated, MPU6050->GyroOffset);
	MPU6050_DegFromGyro(&Gyro, &Gyro.GyroX, &Gyro.GyroY, &Gyro.GyroZ, dt);

	//Read accel data
	MPU6050_DegFromAccel(MPU6050, &AccelRoll, &AccelPitch);

	//place for complementary filter

	return MPU6050_OK;
}







/*

HAL_StatusTypeDef MPU6050_MemRead(MPU6050_t *MPU6050, uint8_t Reg, uint8_t *Data, uint16_t Size)
{
	return HAL_I2C_Mem_Read(MPU6050->hi2c, (MPU6050->address << 1), Reg, I2C_MEMADD_SIZE_8BIT, Data, Size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef MPU6050_MemWrite(MPU6050_t *MPU6050, uint8_t Reg, uint8_t Data)
{
	return HAL_I2C_Mem_Write(MPU6050->hi2c, (MPU6050->address << 1), Reg, I2C_MEMADD_SIZE_8BIT, &Data, 1, HAL_MAX_DELAY);
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

	MPU6050_ReadAcceleration(MPU6050, Accelerations, );

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
*/
