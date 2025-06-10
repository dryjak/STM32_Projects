/*
 * utils.c
 *
 *  Created on: Jun 4, 2025
 *      Author: jakub
 */
#include "main.h"
#include "usart.h"
#include "string.h"


void UartLog(char* Message)
{
	HAL_UART_Transmit(&hlpuart1,(uint8_t*) Message, strlen(Message), 1000);
}


