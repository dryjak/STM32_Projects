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
#include "string.h"
#include "stdio.h"

#include "Encoder.h"
#include "PID_Constant_Speed.h"

#include "motor_simple.h"
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
char Message[128];
int16_t Value;
int16_t LastValue;
int16_t EncoderResolution = 3840;
float SampleTime = 1.0f;

int16_t Delta;

float Angle;
float AngularVelocity;
uint8_t FlagCallback;

Encoder_t Encoder;

uint32_t Sum;
float Velocity;
int16_t Val;

//Motors variable
Motor_t Motor;
uint16_t PWM = 0;

uint8_t Blad;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
float Encoder_Angular_Velocity(int32_t *DELTA, int16_t *Val);

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
  MX_LPUART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  Motor_Init(&Motor, &htim4, TIM_CHANNEL_1, PWM, MOTOR_A_DIR1_GPIO_Port	, MOTOR_A_DIR1_Pin, MOTOR_A_DIR2_GPIO_Port, MOTOR_A_DIR2_Pin);



  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_Delay(100);
  if (HAL_TIM_Base_Start_IT(&htim1) != HAL_OK) {
	  Blad = 1;
  }
  HAL_Delay(100);
  //HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);


  Encoder_Init(&Encoder, &htim3, EncoderResolution, SampleTime);
  HAL_Delay(100);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*
	  Value = __HAL_TIM_GET_COUNTER(&htim3);
	  if (LastValue != Value)
	  {
	  sprintf(Message, "Value = %d\n", Value);
	  HAL_UART_Transmit(&hlpuart1, (uint8_t *) Message, strlen(Message), HAL_MAX_DELAY);
	  }
	  */

	  /*
	  if(FlagCallback == 1)
	  {
		  FlagCallback = 0;

		  sprintf(Message, "Delta = %d\n", Delta);
		  HAL_UART_Transmit(&hlpuart1, (uint8_t *) Message, strlen(Message), HAL_MAX_DELAY);
	  }
		*/
	  Motor_SetRideParameters(&Motor, 70, 1);
	  Motor_Ride(&Motor);
	  HAL_Delay(3000);
  	  Motor_SetRideParameters(&Motor, 80, 0);
  	  Motor_Ride(&Motor);
  	  HAL_Delay(3000);



	  /*
	  LastValue = Value;
	  */
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	FlagCallback = 1;
	Encoder_AngularVelocity(&Encoder, &Angle, &AngularVelocity);

}
float Encoder_Angular_Velocity(int32_t *DELTA, int16_t *Val)
{
	int16_t CurrentValue =  __HAL_TIM_GetCounter(&htim3);
	*Val = CurrentValue;
	static int16_t LastValue = 0;

	int16_t Delta = CurrentValue - LastValue;
	if(Delta > EncoderResolution/2)
	{
		Delta -=  EncoderResolution;
	}
	else if(Delta < -EncoderResolution/2)
	{
		Delta += EncoderResolution;
	}

	float Angle = (360.0 * Delta)/EncoderResolution;
	float AngularVelocity = Angle / SampleTime;
	LastValue = CurrentValue;

	*DELTA = Delta;

	return AngularVelocity;
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
