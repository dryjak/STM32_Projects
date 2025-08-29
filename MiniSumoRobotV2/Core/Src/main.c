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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
uint16_t AdcValues[3][2];
uint8_t Mean = 3;
uint16_t MeanL = 0, MeanR = 0;

uint16_t *SharpPointerL;
uint16_t *SharpPointerR;
//sensors
float SharpVoltageL;
float FilteredVoltageL = 0;


//MotorLVariablesR
Motor_t MotorR;
uint16_t MotorPwmR;

//MotorLVariablesL
Motor_t MotorL;
uint16_t MotorPwmL;

//Sumo Strategy
SumoMotors_t SumoMotors;
SumoSensors_t SumoSensors;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void MeanValue(uint16_t Values[3][2], uint8_t Mean, uint16_t *MeanL, uint16_t *MeanR);

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
//initialize sensors
  //SharpPointerL = &AdcValues[0];
  //SharpPointerR = &AdcValues[1];

//initialize motors
Motor_Init(&MotorR, &htim1, TIM_CHANNEL_1, MotorPwmL, MotorRDir1_GPIO_Port, MotorRDir1_Pin, MotorRDir2_GPIO_Port, MotorRDir2_Pin);
Motor_Init(&MotorL, &htim1, TIM_CHANNEL_2, MotorPwmR, MotorLDir1_GPIO_Port, MotorLDir1_Pin, MotorLDir2_GPIO_Port, MotorLDir2_Pin);

//initialize motors to streategy
Sumo_InitMotors(&SumoMotors, &MotorL, &MotorR);
//initialize sensors to sumo strategy
Sumo_InitDistanceSensors(&SumoSensors, &MeanL , &MeanR);



  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *) AdcValues, 3 * 2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //HAL_Delay(50);
  while (1)
  {

	  //mean value from adc values
	    //MeanValue(AdcValues, Mean, &MeanL, &MeanR);
	  /*
	  uint32_t MeanXtmp = 0;
	  uint32_t MeanYtmp = 0;
	  	  for(uint8_t i = 0; i < 3; i++)
	  	  {
	  		  MeanXtmp += AdcValues[i][0];
	  		  MeanYtmp += AdcValues[i][1];
	  	  }
	  MeanL = MeanXtmp / 3;
	  MeanR = MeanYtmp / 3;
	*/

	  //Calculating voltage from mean value from adc sensors
	 // AdcToVoltage(&MeanL, &SharpVoltageL);
	 // IIRFilter(0.3, SharpVoltageL, &FilteredVoltageL);

	  //testing motors
	  Motor_SetRideParameters(&MotorR, 50, 1);
	  Motor_SetRideParameters(&MotorL, 50, 0);
	  Motor_Ride(&MotorR);
	  Motor_Ride(&MotorL);
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
void MeanValue(uint16_t Values[3][2], uint8_t Mean, uint16_t *MeanL, uint16_t *MeanR)
{
    uint32_t TmpL = 0, TmpR = 0; // używamy 32-bit aby uniknąć przepełnienia
    uint8_t i;

    for(i = 0; i < Mean; i++)
    {
        TmpL += Values[i][0];  // lewy czujnik
        TmpR += Values[i][1];  // prawy czujnik
    }
    *MeanL = TmpL / Mean;
    *MeanR = TmpR / Mean;
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
