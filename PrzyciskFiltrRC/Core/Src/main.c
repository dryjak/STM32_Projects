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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "motor_simple.h"
#include "sumo_strategy.h"
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
uint8_t i;

Motor_t SilnikPrawy;
Motor_t SilnikLewy;
uint8_t PWM;
SumoMotors_t SumoMotors;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);


  Motor_Init(&SilnikPrawy, &htim1, TIM_CHANNEL_1, PWM, Dir1_GPIO_Port, Dir1_Pin, Dir2_GPIO_Port, Dir2_Pin);
  Motor_Init(&SilnikLewy, &htim1, TIM_CHANNEL_2, PWM, DirMotor2V1_GPIO_Port, DirMotor2V1_Pin, DirMotor2V2_GPIO_Port, DirMotor2V2_Pin);
  Sumo_Init(&SumoMotors, &SilnikLewy, &SilnikPrawy);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(HAL_GPIO_ReadPin(CzujnikL_GPIO_Port, CzujnikL_Pin))
	  {
		  HAL_GPIO_WritePin(LedL_GPIO_Port, LedL_Pin, 0);
	  }
	  else
	  {

		  Sumo_SearchLeft(&SumoMotors);

		  HAL_GPIO_WritePin(LedL_GPIO_Port, LedL_Pin, 1);
	  }


	  if(HAL_GPIO_ReadPin(CzujnikS_GPIO_Port, CzujnikS_Pin))
	  {
	  		  HAL_GPIO_WritePin(LedS_GPIO_Port, LedS_Pin, GPIO_PIN_RESET);
	  }
	  else
	  {
		  	  SumoAtack(&SumoMotors);
	  		  HAL_GPIO_WritePin(LedS_GPIO_Port, LedS_Pin, GPIO_PIN_SET);


	  		  /*
	  		  for(i = 0; i < 100; i++)
	  		  {
	  			  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, i);
	  			  HAL_Delay(30);
	  		  }
	  		  for(i = 100; i > 0; i--)
	  		  {
	  			  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, i);
	  			  HAL_Delay(30);

	  		  }*/
	  }


	  if(HAL_GPIO_ReadPin(CzujnikP_GPIO_Port, CzujnikP_Pin))
	  {
	  		  HAL_GPIO_WritePin(LedP_GPIO_Port, LedP_Pin, GPIO_PIN_RESET);
	  }
	  else
	  {
		  Sumo_SearchRight(&SumoMotors);
		  HAL_GPIO_WritePin(LedP_GPIO_Port, LedP_Pin, GPIO_PIN_SET);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
