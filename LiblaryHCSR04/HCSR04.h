/*
 * HCR04.h
 *
 *  Created on: Jun 11, 2025
 *      Author: jakub
 */

#ifndef INC_HCSR04_H_
#define INC_HCSR04_H_

typedef enum
{
	HCSR04_OK,		//0
	HCSR04_ERROR	//1
}HCSR04_Status_t;

typedef struct
{
	TIM_HandleTypeDef 	*htim_trigger;
	TIM_HandleTypeDef 	*htim_echo;

	uint32_t			TrigTimChannel;

	uint32_t			EchoTimChannelStart;
	uint32_t 			EchoTimChannelStop;

	uint16_t 			Timer_Result;

}HCSR04_t;


void HCSR04_Interrupt_Handler(HCSR04_t *hcsr04);

//init function
HCSR04_Status_t HCSR04_Init(HCSR04_t *hcsr04, TIM_HandleTypeDef *htim_trigger, TIM_HandleTypeDef *htim_echo,
		uint32_t trigger_tim_channel,
		uint32_t echo_tim_channel_start, uint32_t echo_tim_channel_stop);

HCSR04_Status_t HCSR04_Get_Distance_F(HCSR04_t *hcsr04, float *Result);


//Wait for response
//HCSR04_Status_t HCSR04_Wait_For_Response(HCSR04_t *hcsr04);

//HCSR04_Status_t HCSR04_Calculate_Distance_Int(HCSR04_t *hcsr04, uint16_t *distance);
//HCSR04_Status_t HCSR04_Calculate_Distance_Float(HCSR04_t *hcsr04, float *distance);





#endif /* INC_HCSR04_H_ */
