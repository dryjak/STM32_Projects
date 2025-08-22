/*
 * NRF24_L1.c
 *
 *  Created on: Aug 22, 2025
 *      Author: jakub
 */
#include "NRF24_L1.h"

void InitNRF24(NRF24_t *NRF24_Module, GPIO_TypeDef *NRF24CE_Port, uint16_t NRF24CE_Pin, GPIO_TypeDef *NRF24CSN_Port, uint16_t NRF24CSN_Pin)
{
	NRF24_Module->NRF24CE_Port = NRF24CE_Port;
	NRF24_Module->NRF24CE_Pin = NRF24CE_Pin;

	NRF24_Module->NRF24CSN_Port = NRF24CSN_Port;
	NRF24_Module->NRF24CSN_Pin = NRF24CSN_Pin;
}


void WriteRegister(uint8_t Register, uint8_t Data)
{

}
