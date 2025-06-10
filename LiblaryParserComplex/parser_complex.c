/*
 * parser_parse.c
 *
 *  Created on: Jun 4, 2025
 *      Author: jakub
 *
 */
#include <main.h>
#include <ring_buffer.h>
#include <string.h>
#include "utils.h"
#include <parser_complex.h>
#include "stdlib.h"
#include "stdio.h"

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
	//LED_OFF
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

//LED=
//    0
//    1
static void ParseSecondPartComplex(void)
{
	char* SecondCommandPart = strtok(NULL, ";");
	if (SecondCommandPart[0] < '0' || SecondCommandPart[0] > '1')
	{
		UartLog("Type LED=0 or LED=1 !\n");
		return;
	}
	if (SecondCommandPart[0] == '0')
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		UartLog("Led is OFF!\n");
	}
	else if (SecondCommandPart[0] == '1')
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin, GPIO_PIN_SET);
		UartLog("Led is ON!\n");
	}
}
//PID=x.xx,y.yy,z.zz
static void ParsePIDPartComplex(void)
{
	int i,j;
	float PID_Data[3];
	uint8_t Message[32];
	for(i = 0; i < 3; i++)
	{
		char* PID_Values = strtok(NULL, ",");
		if(strlen(PID_Values) > 0)
		{
			for(j = 0; PID_Values[j] != 0;j++)
			{
				if((PID_Values[j] < '0' || PID_Values[j] > '9') && PID_Values[j] != '.')
				{
					UartLog("Usage: PID=x.xx,y.yy,z.zz\\n dont use letters\n");
					return;
				}
				PID_Data[i] = atof(PID_Values);
			}
		}
		else
		{
			UartLog("Too less values \n Usage: PID=x.xx,y.yy,z.zz \n");
			return;
		}
	}
	sprintf((char*)Message, "P = %.2f\n", PID_Data[0]);
	UartLog((char*)Message);
	sprintf((char*)Message, "I = %.2f\n", PID_Data[1]);
	UartLog((char*)Message);
	sprintf((char*)Message, "D = %.2f\n", PID_Data[2]);
	UartLog((char*)Message);

}



void ParserParseComplex (uint8_t *FinalTable)
{
	char* FirstCommandPart = strtok((char*) FinalTable, (char*)"=");

	if (strcmp(FirstCommandPart, "LED") == 0)
	{
		ParseSecondPartComplex();
	}
	else if(strcmp(FirstCommandPart, "PID") == 0)
	{
		ParsePIDPartComplex();
	}
}
