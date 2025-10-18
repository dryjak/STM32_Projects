/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor_simple.h"
#include "Encoder.h"
//#include "PID_Constant_Speed.h"
#include "PID.h"
#include "AverageFilter.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//Motor Variables
Motor_t Motor;
int16_t PWM = 10;

//Encoder Variables
Encoder_t Encoder;
uint16_t EncoderResolution = 3840;
float EncoderSampleTime = 0.1;
float EncoderAngle = 0;
float EncoderAngularVelocity = 0;


PID_t PID_Speed;
float P = 23;
float I = 0;
float D = 0;

float AngularVelocitySet = 100.0;
float PID_SpeedSampleTime = 0.1;
float PID_MinValue = 0;
float PID_MaxValue = 100;

uint8_t flag = 0;

float PID_Output;
float PID_Output_Angle;
float PID_Output_Filtered;

float MaxAngularVelocity = 960;

Average_t Average;


char Message[128];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
float MapValues(float MaxValue, float Value);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  //MX_TIM4_Init();
// HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  Motor_Init(&Motor, &htim4, TIM_CHANNEL_1, PWM, MOTOR_DIR1_GPIO_Port,  MOTOR_DIR1_Pin, MOTOR_DIR2_GPIO_Port,  MOTOR_DIR2_Pin);
  Encoder_Init(&Encoder, &htim3, EncoderResolution, EncoderSampleTime);
  HAL_TIM_Base_Start_IT(&htim1);

  PID_Init(&PID_Speed, P, I, D, PID_SpeedSampleTime, MaxAngularVelocity, PID_MinValue);

  Average_Init(&Average);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*
	  HAL_GPIO_WritePin(MOTOR_DIR1_GPIO_Port, MOTOR_DIR1_Pin, 1);
	  HAL_GPIO_WritePin(MOTOR_DIR2_GPIO_Port, MOTOR_DIR2_Pin, 0);
	  TIM4->CCR1 = 30;
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	  HAL_Delay(3000);

	  HAL_GPIO_WritePin(MOTOR_DIR1_GPIO_Port, MOTOR_DIR1_Pin, 0);
	  HAL_GPIO_WritePin(MOTOR_DIR2_GPIO_Port, MOTOR_DIR2_Pin, 1);
	  TIM4->CCR1 = 80;
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	  HAL_Delay(3000);
	  */

/*
	  Motor_SetRideParameters(&Motor, 100, 1);
	  Motor_Ride(&Motor);
	  HAL_Delay(5000);

	  Motor_SetRideParameters(&Motor, 50, 1);
	  Motor_Ride(&Motor);
	  HAL_Delay(5000);
*/



	  if (flag == 1)
	  {
		  flag = 0;
		  PID_Output = PID_Compute(&PID_Speed, EncoderAngularVelocity, AngularVelocitySet);

		  Average_Calculate(&Average, PID_Output, &PID_Output_Filtered);

		  PID_Output_Angle = MapValues(960.0, PID_Output_Filtered);

		  Motor_SetRideParameters(&Motor, PID_Output_Angle, 1);

		  Motor_Ride(&Motor);

		  sprintf(Message, "%.3f \n", PID_Output_Filtered);
		  HAL_UART_Transmit(&hlpuart1, (uint8_t *) Message, strlen(Message), HAL_MAX_DELAY);
	  }




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM1_UP_TIM16_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}

/* USER CODE BEGIN 4 */
float MapValues(float MaxValue, float Value)
{
	float X = (float)(Value * 100.0) / (float)MaxValue;
	return X;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		//EncoderAngle = 0;
		//EncoderAngularVelocity = 0;
		Encoder_AngularVelocity(&Encoder, &EncoderAngle, &EncoderAngularVelocity);

		flag = 1;

	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
