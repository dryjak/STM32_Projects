/*
 * NRF24_L1.c
 *
 *  Created on: Aug 22, 2025
 *      Author: jakub
 */
#include "NRF24_L1.h"

//extern SPI_HandleTypeDef hspi2;



void CS_Select(NRF24_t *NRF24_Module)
{
	HAL_GPIO_WritePin(NRF24_Module->NRF24CSN_Port, NRF24_Module->NRF24CSN_Pin, GPIO_PIN_RESET);
}

void CS_Unselect(NRF24_t *NRF24_Module)
{
	HAL_GPIO_WritePin(NRF24_Module->NRF24CSN_Port, NRF24_Module->NRF24CSN_Pin, GPIO_PIN_SET);
}
void CE_Enable(NRF24_t *NRF24_Module)
{
	HAL_GPIO_WritePin(NRF24_Module->NRF24CE_Port, NRF24_Module->NRF24CE_Pin, GPIO_PIN_RESET);
}
void CE_Disable(NRF24_t *NRF24_Module)
{
	HAL_GPIO_WritePin(NRF24_Module->NRF24CE_Port, NRF24_Module->NRF24CE_Pin, GPIO_PIN_SET);
}


//write a single byte to a particular register
void Nrf24_WriteRegister(NRF24_t *NRF24_Module, uint8_t Register, uint8_t Data)
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
void Nrf24_WriteMultiRegister(NRF24_t *NRF24_Module, uint8_t Register, uint8_t *Data, uint8_t Size)
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

uint8_t Nrf24_ReadRegister(NRF24_t *NRF24_Module, uint8_t Register)
{
	uint8_t Data = 0;
	//Pull cs pin low to select the device
	CS_Select(NRF24_Module);

	HAL_SPI_Transmit(NRF24_Module->NRF24_SPI, &Register, 1, 1000);
	HAL_SPI_Receive(NRF24_Module->NRF24_SPI, &Data, 1, 1000);

	//Pull cs pin high to release the device
	CS_Unselect(NRF24_Module);

	return Data;
}


void Nrf24_ReadMultipleRegister(NRF24_t *NRF24_Module, uint8_t Register, uint8_t Size, uint8_t *Data)
{
	//Pull cs pin low to select the device
	CS_Select(NRF24_Module);

	HAL_SPI_Transmit(NRF24_Module->NRF24_SPI, &Register, 1, 1000);
	HAL_SPI_Receive(NRF24_Module->NRF24_SPI, Data, Size, 1000);

	//Pull cs pin high to release the device
	CS_Unselect(NRF24_Module);
}

void Nrf24_SendCommand(NRF24_t *NRF24_Module, uint8_t Cmd)
{
		//Pull cs pin low to select the device
		CS_Select(NRF24_Module);

		HAL_SPI_Transmit(NRF24_Module->NRF24_SPI, &Cmd, 1, 1000);

		//Pull cs pin high to release the device
		CS_Unselect(NRF24_Module);
}

void Nrf24_InitNRF24(NRF24_t *NRF24_Module, GPIO_TypeDef *NRF24CE_Port, uint16_t NRF24CE_Pin, GPIO_TypeDef *NRF24CSN_Port, uint16_t NRF24CSN_Pin, SPI_HandleTypeDef *hspi)
{
	NRF24_Module->NRF24_SPI = hspi;

	NRF24_Module->NRF24CE_Port = NRF24CE_Port;
	NRF24_Module->NRF24CE_Pin = NRF24CE_Pin;

	NRF24_Module->NRF24CSN_Port = NRF24CSN_Port;
	NRF24_Module->NRF24CSN_Pin = NRF24CSN_Pin;

	//dissable the chip before cofiguring the device
	CE_Disable(NRF24_Module);
	//CS_Unselect(NRF24_Module);

	Nrf24_WriteRegister(NRF24_Module, CONFIG, 0);			//will be configured later
	Nrf24_WriteRegister(NRF24_Module, EN_AA, 0);			//No auto ack
	Nrf24_WriteRegister(NRF24_Module, EN_RXADDR, 0);		//Nt enabling any data pipe now
	Nrf24_WriteRegister(NRF24_Module, SETUP_AW, 0x03);		//5 bytes for the tx / rx address
	Nrf24_WriteRegister(NRF24_Module, SETUP_RETR, 0x00);	//no Automatic Retransmission
	Nrf24_WriteRegister(NRF24_Module, RF_CH, 0x00);			//will be setup with rx or tx
	Nrf24_WriteRegister(NRF24_Module, RF_SETUP, 0x0E);		//Power = 0dB, dataRate = 2Mbps

	//enabling the chip after configuration
	CE_Enable(NRF24_Module);
	//CS_Select(NRF24_Module);
}

//Set up the TX mode
void Nrf24_TxMode(NRF24_t *NRF24_Module, uint8_t *Address, uint8_t channel)
{
	//disable the chip before configuring the device
	CE_Disable(NRF24_Module);
	CS_Unselect(NRF24_Module);

	Nrf24_WriteRegister(NRF24_Module, RF_CH, channel);
	Nrf24_WriteMultiRegister(NRF24_Module, TX_ADDR, Address, 5);	//write the transmit adress

	uint8_t Config = Nrf24_ReadRegister(NRF24_Module, CONFIG);
	Config = Config | (1<<1);
	Nrf24_WriteRegister(NRF24_Module, CONFIG, Config);

	//enabling the chip after configuration
	CE_Enable(NRF24_Module);
}


