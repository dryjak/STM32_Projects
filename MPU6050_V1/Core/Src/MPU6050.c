/*
 * MPU6050.c
 *
 *  Created on: Sep 6, 2025
 *      Author: jakub
 */

#include "MPU6050.h"
#include "main.h"
#include "math.h"

// TO DO: odczyt z accelerometru
typedef union {
    struct {
        uint8_t ByteLow;
        uint8_t ByteHigh;
    } Byte;
    int16_t Var16u;
} ConvTo16_t;

// --- Funkcje pomocnicze (tylko w tym pliku .c) ---
static uint8_t Read8(MPU6050_t *MPU6050, uint8_t Register);
static MPU6050_STATE_t Write8(MPU6050_t *MPU6050, uint8_t Register, uint8_t Value);
static int16_t Read16(MPU6050_t *MPU6050, uint8_t Register);

static MPU6050_STATE_t MPU6050_SetGyroRange(MPU6050_t *MPU6050);
static MPU6050_STATE_t MPU6050_SetAccelerationRange(MPU6050_t *MPU6050);

static MPU6050_STATE_t MPU6050_ReadAccelerationRaw(MPU6050_t *MPU6050, AccelRaw_t *AccelRaw);
static MPU6050_STATE_t MPU6050_ReadAcceleration(MPU6050_t *MPU6050, Accel_t *Accelerations, AccelOffset_t AccelOffset);
static MPU6050_STATE_t MPU6050_CalibrateAccel(MPU6050_t *MPU6050, AccelOffset_t *AccelOffset);

static MPU6050_STATE_t MPU6050_ReadGyroRaw(MPU6050_t *MPU6050, GyroRaw_t *GyroRaw);
static MPU6050_STATE_t MPU6050_ReadGyro(MPU6050_t *MPU6050, Gyro_t *GyroCalculated, GyroOffset_t GyroOffset);
static MPU6050_STATE_t MPU6050_CalibrateGyro(MPU6050_t *MPU6050, GyroOffset_t *GyroOffset);

// --- Funkcje publiczne (API) ---

MPU6050_STATE_t MPU6050_Init(MPU6050_t *MPU6050, I2C_HandleTypeDef *Hi2c, uint16_t Address)
{
    uint8_t CheckID;

    MPU6050->hi2c     = Hi2c;
    MPU6050->address  = Address;

    CheckID = Read8(MPU6050, 0x75);
    if (CheckID != 0x68) {
        return MPU6050_ERROR;
    }

    if (MPU6050_WakeUp(MPU6050) != MPU6050_OK) {
        return MPU6050_ERROR;
    }
    if (MPU6050_SetAccelerationRange(MPU6050) != MPU6050_OK) {
        return MPU6050_ERROR;
    }
    if (MPU6050_SetGyroRange(MPU6050) != MPU6050_OK) {
        return MPU6050_ERROR;
    }

    MPU6050_CalibrateAccel(MPU6050, &MPU6050->AccelOffset);
    MPU6050_CalibrateGyro(MPU6050, &MPU6050->GyroOffset);


    return MPU6050_OK;
}

uint8_t MPU6050_WHO_AM_I (MPU6050_t *MPU6050)
{
    return Read8(MPU6050, 0x75);
}

MPU6050_STATE_t MPU6050_WakeUp(MPU6050_t *MPU6050)
{
    uint8_t Value = Read8(MPU6050, PWR_MGMT_1);

    Value &= ~(1 << 6);  // disable sleep
    Value &= ~(1 << 5);  // disable cycle
    Value |= (1 << 3);   // disable temp sensor

    return Write8(MPU6050, PWR_MGMT_1, Value);
}

// --- Obliczanie kątów ---

MPU6050_STATE_t MPU6050_DegFromAccel(MPU6050_t *MPU6050, float *Roll, float *Pitch)
{
    Accel_t Accel;
    MPU6050_ReadAcceleration(MPU6050, &Accel, MPU6050->AccelOffset);

    *Roll  = atan2f(Accel.AccelY, Accel.AccelZ) * 180.0f / M_PI;
    *Pitch = atan2f(-Accel.AccelX, sqrtf(Accel.AccelY*Accel.AccelY + Accel.AccelZ*Accel.AccelZ)) * 180.0f / M_PI;

    return MPU6050_OK;
}

MPU6050_STATE_t MPU6050_DegFromGyro(MPU6050_t *MPU6050, float *RollG, float *PitchG, float *YawG, float dt)
{
    Gyro_t Gyro;
    MPU6050_ReadGyro(MPU6050, &Gyro, MPU6050->GyroOffset);

    *RollG  += Gyro.GyroX * dt;
    *PitchG += Gyro.GyroY * dt;
    *YawG   += Gyro.GyroZ * dt;

    return MPU6050_OK;
}

MPU6050_STATE_t MPU6050_Angle(MPU6050_t *MPU6050, float *Roll, float *Pitch, float *Yaw, float dt)
{
    float RollAccel, PitchAccel;

    // Accel
    MPU6050_DegFromAccel(MPU6050, &RollAccel, &PitchAccel);

    // Gyro
    MPU6050_DegFromGyro(MPU6050, Roll, Pitch, Yaw, dt);

    // Komplementarny filtr
    const float alpha = 0.98f;
    *Roll  = alpha * (*Roll)  + (1.0f - alpha) * RollAccel;
    *Pitch = alpha * (*Pitch) + (1.0f - alpha) * PitchAccel;

    return MPU6050_OK;
}

// --- Implementacja funkcji pomocniczych ---

static uint8_t Read8(MPU6050_t *MPU6050, uint8_t Register)
{
    uint8_t Value;
    HAL_I2C_Mem_Read(MPU6050->hi2c, (MPU6050->address) << 1, Register, 1, &Value, 1, MPU6050_TIMEOUT);
    return Value;
}

static MPU6050_STATE_t Write8(MPU6050_t *MPU6050, uint8_t Register, uint8_t Value)
{
    return HAL_I2C_Mem_Write(MPU6050->hi2c, (MPU6050->address) << 1, Register, 1, &Value, 1, MPU6050_TIMEOUT);
}

static int16_t Read16(MPU6050_t *MPU6050, uint8_t Register)
{
    uint8_t Value[2];
    HAL_I2C_Mem_Read(MPU6050->hi2c, (MPU6050->address) << 1, Register, 1, Value, 2, MPU6050_TIMEOUT);
    return (int16_t)((Value[0] << 8) | Value[1]);
}

static MPU6050_STATE_t MPU6050_SetGyroRange(MPU6050_t *MPU6050)
{
    uint8_t RegisterValue = Read8(MPU6050, GYRO_CONFIG);
    RegisterValue &= ~( (1 << 4) | (1 << 5) ); // ±250°/s
    return Write8(MPU6050, GYRO_CONFIG, RegisterValue);
}

static MPU6050_STATE_t MPU6050_SetAccelerationRange(MPU6050_t *MPU6050)
{
    uint8_t RegisterValue = Read8(MPU6050, ACCEL_CONFIG);
    RegisterValue &= ~( (1 << 4) | (1 << 5) ); // ±2g
    return Write8(MPU6050, ACCEL_CONFIG, RegisterValue);
}

// --- surowe i skalowane odczyty Accel/Gyro (prywatne) ---

static MPU6050_STATE_t MPU6050_ReadAccelerationRaw(MPU6050_t *MPU6050, AccelRaw_t *AccelRaw)
{
    AccelRaw->AccelX = Read16(MPU6050, ACCEL_XOUT_H);
    AccelRaw->AccelY = Read16(MPU6050, ACCEL_YOUT_H);
    AccelRaw->AccelZ = Read16(MPU6050, ACCEL_ZOUT_H);
    return MPU6050_OK;
}

static MPU6050_STATE_t MPU6050_ReadAcceleration(MPU6050_t *MPU6050, Accel_t *Accelerations, AccelOffset_t AccelOffset)
{
    AccelRaw_t Raw;
    MPU6050_ReadAccelerationRaw(MPU6050, &Raw);

    const float ScaleFactor = 16384.0f; // ±2g

    Accelerations->AccelX = ((float)Raw.AccelX - AccelOffset.OffsetX) / ScaleFactor;
    Accelerations->AccelY = ((float)Raw.AccelY - AccelOffset.OffsetY) / ScaleFactor;
    Accelerations->AccelZ = ((float)Raw.AccelZ - AccelOffset.OffsetZ) / ScaleFactor;

    return MPU6050_OK;
}

static MPU6050_STATE_t MPU6050_CalibrateAccel(MPU6050_t *MPU6050, AccelOffset_t *AccelOffset)
{
    // TODO: implementacja kalibracji
    AccelOffset->OffsetX = 0;
    AccelOffset->OffsetY = 0;
    AccelOffset->OffsetZ = 0;
    return MPU6050_OK;
}

static MPU6050_STATE_t MPU6050_ReadGyroRaw(MPU6050_t *MPU6050, GyroRaw_t *GyroRaw)
{
    GyroRaw->GyroX = Read16(MPU6050, GYRO_XOUT_H);
    GyroRaw->GyroY = Read16(MPU6050, GYRO_YOUT_H);
    GyroRaw->GyroZ = Read16(MPU6050, GYRO_ZOUT_H);
    return MPU6050_OK;
}

static MPU6050_STATE_t MPU6050_ReadGyro(MPU6050_t *MPU6050, Gyro_t *GyroCalculated, GyroOffset_t GyroOffset)
{
    GyroRaw_t Raw;
    MPU6050_ReadGyroRaw(MPU6050, &Raw);

    const float ScaleFactor = 131.0f; // ±250°/s

    GyroCalculated->GyroX = ((float)Raw.GyroX - GyroOffset.OffsetX) / ScaleFactor;
    GyroCalculated->GyroY = ((float)Raw.GyroY - GyroOffset.OffsetY) / ScaleFactor;
    GyroCalculated->GyroZ = ((float)Raw.GyroZ - GyroOffset.OffsetZ) / ScaleFactor;

    return MPU6050_OK;
}

static MPU6050_STATE_t MPU6050_CalibrateGyro(MPU6050_t *MPU6050, GyroOffset_t *GyroOffset)
{
    // TODO: implementacja kalibracji
    GyroOffset->OffsetX = 0;
    GyroOffset->OffsetY = 0;
    GyroOffset->OffsetZ = 0;
    return MPU6050_OK;
}
