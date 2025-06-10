/*
 * parser_parse.c
 *
 *  Created on: Jun 4, 2025
 *      Author: jakub
 *
 */
#include <main.h>
#include "utils.h"
#include <ring_buffer.h>
#include <parser_simple.h>
#include <string.h>

//function that checks if there is available line (command) to read and it parse this command
void ParserTakeLine(TRingBuffer *Buff, uint8_t *FinalTable)
{
	uint8_t i = 0;
	uint8_t Tmp;
	do
	{
		RB_Read(Buff,&Tmp);
		if(Tmp == ENDLINE)
		{
		  FinalTable[i] = 0;
		}
		else
		{
		  FinalTable[i] = Tmp;
		}

		i++;
	}while(Tmp != ENDLINE); //repeat untill we find the end of the line

}

void ParserParse(uint8_t *FinalTable)
{
	//LED_ON
			  if(strcmp("LED_ON", (char*)FinalTable) == 0)
			  {
				  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				  UartLog("Led is on!\n");
			  }
			  else if(strcmp("LED_OFF", (char*)FinalTable) == 0)
			  {
				  HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin, GPIO_PIN_RESET);
				  UartLog("Led is off!\n");
			  }
}
