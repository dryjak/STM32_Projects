/*
 * NRF24_L1.c
 *
 *  Created on: Aug 22, 2025
 *      Author: jakub
 */
#include "NRF24_L1.h"

//extern SPI_HandleTypeDef hspi2;

void InitNRF24(NRF24_t *NRF24_Module, GPIO_TypeDef *NRF24CE_Port, uint16_t NRF24CE_Pin, GPIO_TypeDef *NRF24CSN_Port, uint16_t NRF24CSN_Pin, SPI_HandleTypeDef *hspi)
{
	NRF24_Module->NRF24_SPI = hspi;

	NRF24_Module->NRF24CE_Port = NRF24CE_Port;
	NRF24_Module->NRF24CE_Pin = NRF24CE_Pin;

	NRF24_Module->NRF24CSN_Port = NRF24CSN_Port;
	NRF24_Module->NRF24CSN_Pin = NRF24CSN_Pin;
}

void CS_Select(NRF24_t *NRF24_Module)
{
	HAL_GPIO_WritePin(NRF24_Module->NRF24CSN_Port, NRF24_Module->NRF24CSN_Pin, GPIO_PIN_RESET);
}

void CS_Unselect(NRF24_t *NRF24_Module)
{
	HAL_GPIO_WritePin(NRF24_Module->NRF24CSN_Port, NRF24_Module->NRF24CSN_Pin, GPIO_PIN_SET);
}
void CE_Select(NRF24_t *NRF24_Module)
{
	HAL_GPIO_WritePin(NRF24_Module->NRF24CE_Port, NRF24_Module->NRF24CE_Pin, GPIO_PIN_RESET);
}
void CE_Unselect(NRF24_t *NRF24_Module)
{
	HAL_GPIO_WritePin(NRF24_Module->NRF24CE_Port, NRF24_Module->NRF24CE_Pin, GPIO_PIN_SET);
}


//write a single byte to a particular register
void WriteRegister(NRF24_t *NRF24_Module, uint8_t Register, uint8_t Data)
{
	uint8_t buff[2];
	buff[0] = Register | 1 << 5;
	buff[1] = Data;

	//Pull cs pin low to select the device
	CS_Select(NRF24_Module);

	HAL_SPI_Transmit(NRF24_Module->NRF24_SPI, buff, 2, 1000);

	//Pull cs pin high to release the device
	CS_Unselect(NRF24_Module);

}


//write multiple bytes to a particular register
void WriteMultiRegister(NRF24_t *NRF24_Module, uint8_t Register, uint8_t *Data, uint8_t Size)
{
	uint8_t buff[2];
	buff[0] = Register | 1 << 5;
	//buff[1] = Data;

	//Pull cs pin low to select the device
	CS_Select(NRF24_Module);

	HAL_SPI_Transmit(NRF24_Module->NRF24_SPI, buff, 1, 1000);
	HAL_SPI_Transmit(NRF24_Module->NRF24_SPI, Data, Size, 1000);

	//Pull cs pin high to release the device
	CS_Unselect(NRF24_Module);

}
