	/*
 * NRF24_L1.h
 *
 *  Created on: Aug 22, 2025
 *      Author: jakub
 */

#ifndef INC_NRF24_L1_H_
#define INC_NRF24_L1_H_

#include "main.h"
//#include "stm32f4xx_hal.h"


typedef struct{
	GPIO_TypeDef 	*NRF24CE_Port;
	uint16_t 		NRF24CE_Pin;

	GPIO_TypeDef 	*NRF24CSN_Port;
	uint16_t 		NRF24CSN_Pin;
}NRF24_t;

void InitNRF24(NRF24_t *NRF24_Module, GPIO_TypeDef *NRF24CE_Port, uint16_t NRF24CE_Pin, GPIO_TypeDef *NRF24CSN_Port, uint16_t NRF24CSN_Pin);

void WriteRegister(uint8_t Register, uint8_t Data);


#endif /* INC_NRF24_L1_H_ */


