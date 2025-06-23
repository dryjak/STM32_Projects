/*_
 * HCR04.c
 *
 *  Created on: Jun 11, 2025
 *      Author: jakub
 */

//TO DO:
//Once more check device conviguration tool
//Probably something wrong with timer conviguration/Echo/Trig

#include "main.h"
#include "tim.h"
#include "HCSR04.h"

#define VelocityConstant ((float)0.01715)	//velocity of sound in air / 2 [cm/us]

static HCSR04_Status_t HCSR04_Calculate_Distance_Float(HCSR04_t *hcsr04, float *distance)
{
	*distance = (float)hcsr04->Timer_Result * VelocityConstant;

	return HCSR04_OK;
}

static HCSR04_Status_t HCSR04_Calculate_Distance_Int(HCSR04_t *hcsr04, uint16_t *distance)
{
	*distance = hcsr04->Timer_Result / 58;

	return HCSR04_OK;
}

HCSR04_Status_t HCSR04_Get_Distance_F(HCSR04_t *hcsr04, float *Result)
{
	HCSR04_Calculate_Distance_Float(hcsr04, Result);

	return HCSR04_OK;
}

void HCSR04_Interrupt_Handler(HCSR04_t *hcsr04)
{
	hcsr04->Timer_Result = __HAL_TIM_GET_COMPARE(hcsr04->htim_echo, hcsr04->EchoTimChannelStop) - __HAL_TIM_GET_COMPARE(hcsr04->htim_echo, hcsr04->EchoTimChannelStart);
}

HCSR04_Status_t HCSR04_Init(HCSR04_t *hcsr04, TIM_HandleTypeDef *htim_trigger, TIM_HandleTypeDef *htim_echo,
		uint32_t trigger_tim_channel,
		uint32_t echo_tim_channel_start, uint32_t echo_tim_channel_stop)
{
	hcsr04->htim_trigger = htim_trigger;
	hcsr04->htim_echo = htim_echo;

	hcsr04->TrigTimChannel = trigger_tim_channel;

	hcsr04->EchoTimChannelStart = echo_tim_channel_start;
	hcsr04->EchoTimChannelStop = echo_tim_channel_stop;

	//HAL_GPIO_WritePin(hcsr04->Trigger_Port, hcsr04->Trigger_Pin, GPIO_PIN_RESET);
	//hcsr04->Timer_Value = timer_value;

	HAL_TIM_Base_Start(hcsr04->htim_echo);
	HAL_TIM_IC_Start(hcsr04->htim_echo, hcsr04->EchoTimChannelStart);
	HAL_TIM_IC_Start_IT(hcsr04->htim_echo, hcsr04->EchoTimChannelStop);

	HAL_TIM_Base_Start(hcsr04->htim_trigger);
	HAL_TIM_PWM_Start(hcsr04->htim_trigger, hcsr04->TrigTimChannel);

	return HCSR04_OK;
}


