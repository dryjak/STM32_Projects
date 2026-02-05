/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Button.h"
#include "SSD1306_OLED.h"
#include "GFX_BlackWhite.h"
#include "fonts/fonts.h"
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
uint32_t TimerDebounce = 50;
uint32_t TimerLongPress = 2000;
uint32_t TimerRepeat = 500;

Button_t ButtonMiddle;
Button_t ButtonTop;
Button_t ButtonBottom;
uint8_t Length;

typedef enum{
	NORMAL_MODE = 0,
	EDIT_MODE
}SystemMode_t;

typedef enum {
    TARGET_WORK = 0,
    TARGET_RELAX
} EditTarget_t;

typedef struct{
	int16_t WorkTime;
	int16_t RelaxTime;
	uint8_t DispalyNeedsUpdate;

	SystemMode_t CurrentMode;
	EditTarget_t   EditTarget;

}ApplicationData_t;

//init default values
ApplicationData_t App = {25, 5, 1, NORMAL_MODE, TARGET_WORK};

char Buffer[32];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ToggleLed();
void TurnOnLed();
void TurnOffLed();

//Button Top
void ButtonTopPress();
void ButtonTopLongPress();
void ButtonTopRepeat();

//Button Bottom
void ButtonBottomPress();
void ButtonBottomLongPress();
void ButtonBottomRepeat();

//Button Middle
void ButtonMidLongPress();
void ButtonMidPress();



//Function to update display
void UpdateDisplay();
void DrawMenuItem(uint8_t y, uint8_t w, uint8_t IsSelected, uint16_t Value, char* Label);

//Button Middle

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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  //Init middle button
  ButtonInit(&ButtonMiddle, ButtonMiddle_GPIO_Port, ButtonMiddle_Pin, TimerDebounce,TimerLongPress, TimerRepeat);
  ButtonRegisterPressCallback(&ButtonMiddle, ButtonMidPress);
  ButtonRegisterLongPressCallback(&ButtonMiddle, ButtonMidLongPress);
  ButtonRegisterRepeatCallback(&ButtonMiddle, ToggleLed);
  ButtonRegisterGoToIdleCallback(&ButtonMiddle, TurnOffLed);

  //Init top button
  ButtonInit(&ButtonTop, ButtonTop_GPIO_Port, ButtonTop_Pin, TimerDebounce, TimerLongPress, TimerRepeat);
  ButtonRegisterPressCallback(&ButtonTop, ButtonTopPress);
  ButtonRegisterLongPressCallback(&ButtonTop, ButtonTopLongPress);
  ButtonRegisterRepeatCallback(&ButtonTop, ButtonTopRepeat);
  ButtonRegisterGoToIdleCallback(&ButtonTop, TurnOffLed);

  //Init Bottom Button
  ButtonInit(&ButtonBottom, ButtonBottom_GPIO_Port, ButtonBottom_Pin, TimerDebounce, TimerLongPress, TimerRepeat);
  ButtonRegisterPressCallback(&ButtonBottom, ButtonBottomPress);
  ButtonRegisterLongPressCallback(&ButtonBottom, ButtonBottomLongPress);
  ButtonRegisterRepeatCallback(&ButtonBottom, ButtonBottomRepeat);
  ButtonRegisterGoToIdleCallback(&ButtonBottom, TurnOffLed);


  GFX_SetFont(font_8x5);
  SSD1306_Init(&hi2c1);
  UpdateDisplay();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 // HAL_GPIO_WritePin( , GPIO_Pin, PinState), GPIO_Pin, PinState);
	 ButtonTask (&ButtonMiddle);
	 ButtonTask (&ButtonTop);
	 ButtonTask (&ButtonBottom);
	 if(App.DispalyNeedsUpdate == 1)
	 {
		 App.DispalyNeedsUpdate = 0;
		 UpdateDisplay();
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void UpdateDisplay()
{
	if(App.CurrentMode == NORMAL_MODE)
	{
		SSD1306_Clear(BLACK);
		GFX_DrawRectangle(0, 0, 128, 63, PIXEL_WHITE);

		sprintf(Buffer, "WORK: %d", App.WorkTime);
		GFX_DrawString(10,5, Buffer, PIXEL_WHITE, 0);

		sprintf(Buffer, "RELAX: %d", App.RelaxTime);
		GFX_DrawString(10,18, Buffer, PIXEL_WHITE, 0);

		GFX_DrawString(10,31, "START", PIXEL_WHITE, 0);
		SSD1306_Display();
	}
	else
	{
		if(App.EditTarget == TARGET_WORK)
		{
			SSD1306_Clear(BLACK);
			//GFX_DrawRectangle(0, 0, 128, 63, PIXEL_WHITE);;
			GFX_SetFont(font_8x5);

			Length = sprintf(Buffer, "WORK: %d", App.WorkTime);
			GFX_DrawFillRectangle(8, 3, (Length + 1) * 8, 12, PIXEL_WHITE);
			GFX_DrawString(10,5, Buffer, PIXEL_BLACK, 1);


			sprintf(Buffer, "RELAX: %d", App.RelaxTime);
			GFX_DrawString(10,18, Buffer, PIXEL_WHITE, 0);

			SSD1306_Display();
		}
		else
		{
			SSD1306_Clear(BLACK);
			//GFX_DrawRectangle(0, 0, 128, 63, PIXEL_WHITE);
			GFX_SetFont(font_8x5);

			sprintf(Buffer, "WORK: %d", App.WorkTime);
			GFX_DrawString(10,5, Buffer, PIXEL_WHITE, 0);

			Length = sprintf(Buffer, "RELAX: %d", App.RelaxTime);
			GFX_DrawFillRectangle(8, 16, Length * 8, 12, PIXEL_WHITE);
			GFX_DrawString(10,18, Buffer, PIXEL_BLACK, 1);

			SSD1306_Display();
		}
	}

}
void DrawMenuItem(uint8_t y, uint8_t w, uint8_t IsSelected, uint16_t Value, char* Label)
{
	char TempBuffer[32];
	uint8_t Length;

	Length = sprintf(TempBuffer, "%s: %d", Label, Value);

	if(IsSelected)
	{
		GFX_DrawFillRectangle(8, y - 2, Length * 6 + 4, 8 + 4, PIXEL_WHITE);
		GFX_DrawString(10, y, TempBuffer, PIXEL_BLACK, 1);	//black text
	}
	else
	{
		GFX_DrawString(2, y, TempBuffer, PIXEL_WHITE, 0);	//white text
	}
}
void ModifyWorkTime(int8_t ChangeTimeAmount)
{
	if(App.CurrentMode != EDIT_MODE) return;
	if(App.EditTarget == TARGET_WORK)
	{
		App.WorkTime += ChangeTimeAmount;
	}
	else
	{
		App.RelaxTime += ChangeTimeAmount;
	}

	if (App.WorkTime < 0){App.WorkTime = 0;}
	else if (App.WorkTime > 999){App.WorkTime = 999;}
	if (App.RelaxTime < 0){App.RelaxTime = 0;}
	else if (App.RelaxTime > 999){App.RelaxTime = 999;}
	App.DispalyNeedsUpdate = 1;
}

void ButtonMidPress()
{
	TurnOnLed();
	if(App.CurrentMode == NORMAL_MODE) //do start stop
	{
		TurnOnLed();
	}
	else //do change mode
	{

		if(App.EditTarget == TARGET_WORK)
		{
			App.EditTarget = TARGET_RELAX;	//change to target relax
		}
		else
		{
			App.EditTarget = TARGET_WORK;	//change to target work
		}
		App.DispalyNeedsUpdate = 1;
	}

}
void ButtonMidLongPress()
{
	TurnOffLed();
	if(App.CurrentMode == NORMAL_MODE)
	{
		App.CurrentMode = EDIT_MODE;
		App.EditTarget = TARGET_WORK;
	}
	else
	{
		App.CurrentMode = NORMAL_MODE;
	}
	App.DispalyNeedsUpdate = 1;
}
void ButtonTopPress()
{
	TurnOnLed();

	ModifyWorkTime(1);
}
void ButtonTopLongPress()
{
	TurnOffLed();
	ModifyWorkTime(4);
}
void ButtonTopRepeat()
{
	ToggleLed();
	ModifyWorkTime(5);
}
void ButtonBottomPress()
{
	TurnOnLed();
	ModifyWorkTime(-1);
}
void ButtonBottomLongPress()
{
	TurnOffLed();
	ModifyWorkTime(-4);
}
void ButtonBottomRepeat()
{
	ToggleLed();
	ModifyWorkTime(-5);
}

void ToggleLed()
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
void TurnOnLed()
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}
void TurnOffLed()
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_RESET);
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
