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
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Button.h"
#include "SSD1306_OLED.h"
#include "GFX_BlackWhite.h"
#include "fonts/fonts.h"
#include "stdio.h"
#include "PomodoroFSM.h"
#include "time.h"
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

char Buffer[32];

RTC_TimeTypeDef Time;
RTC_DateTypeDef Date;

uint8_t Seconds;
uint8_t Minutes;
uint8_t Hours;
char TimeBuffer[16];

//Pomodoro FSM variables
Pomodoro_t Pomodoro;
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
void DrawMenuItem(uint8_t y, char* Label, uint16_t Value, uint8_t IsSelected);

int32_t GetCurrentUnixTime(RTC_HandleTypeDef *hrtc);
void ChangeTimeFormat(int32_t TimeInSeconds, uint8_t *Hours, uint8_t *Minutes, uint8_t *Seconds);



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
  MX_RTC_Init();
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

  //Pomodoro FSM init
  PomodoroInit(&Pomodoro);

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

	 //get current time
	 int32_t Now = GetCurrentUnixTime(&hrtc);
	 //pomodoro logic
	 PomodoroTask(&Pomodoro, Now);
	 if(Pomodoro.NeedsRedraw == 1)
	 {
		 Pomodoro.NeedsRedraw = 0;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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
	switch(Pomodoro.CurrentState)
	{
	case POMO_STATE_IDLE:		//draw idle
			SSD1306_Clear(BLACK);
			//GFX_DrawRectangle(0, 0, 128, 63, PIXEL_WHITE);

			GFX_DrawString(10, 5, "CLICK TO START", PIXEL_WHITE, 0);

			//DrawMenuItem(17, "WORK", Pomodoro.CfgWorkTime, 0);
			//draw remaining time
			ChangeTimeFormat(Pomodoro.CfgWorkTime, &Hours, &Minutes, &Seconds);
			sprintf(Buffer,"WORK: %s", TimeBuffer);
			GFX_DrawString(10, 17, Buffer, PIXEL_WHITE, 0);



			//DrawMenuItem(29, "RELAX", Pomodoro.CfgRelaxTime, 0);
			//GFX_DrawString(10, 5, "CLICK TO START", PIXEL_WHITE, 0);

			ChangeTimeFormat(Pomodoro.CfgRelaxTime, &Hours, &Minutes, &Seconds);
			sprintf(Buffer,"RELAX: %s", TimeBuffer);
			GFX_DrawString(10, 29, Buffer, PIXEL_WHITE, 0);
			//DrawMenuItem(41, "START / IDLE", 0, 0);

			SSD1306_Display();
		break;
	case POMO_STATE_RUNNING:	//draw running
			SSD1306_Clear(BLACK);
			//GFX_DrawRectangle(0, 0, 128, 63, PIXEL_WHITE);
			if (Pomodoro.CurrentPhase == POMO_PHASE_WORK)
			{
				GFX_DrawString(10, 5, "WORK REMAINED", PIXEL_WHITE, 0);
			}
			else if(Pomodoro.CurrentPhase == POMO_PHASE_RELAX)
			{
				GFX_DrawString(10, 5, "RELAX REMAINED", PIXEL_WHITE, 0);
			}

			//draw remaining time
			ChangeTimeFormat(Pomodoro.TimeToDisplay, &Hours, &Minutes, &Seconds);
			GFX_DrawString(10, 20, TimeBuffer, PIXEL_WHITE, 0);

/*
			if (Pomodoro.CurrentPhase == POMO_PHASE_WORK)
			{
				DrawMenuItem(17, "WORK REMAINED", Pomodoro.TimeToDisplay, 0);
			}
			else if(Pomodoro.CurrentPhase == POMO_PHASE_RELAX)
			{
				DrawMenuItem(17, "RELAX REMAINED", Pomodoro.TimeToDisplay, 0);
			}
*/
			SSD1306_Display();
		break;
	case POMO_STATE_PAUSED:		//draw paused
			SSD1306_Clear(BLACK);

			//GFX_DrawString(10, 5, "PAUSED", PIXEL_WHITE, 0);

			if (Pomodoro.CurrentPhase == POMO_PHASE_WORK)
			{
				GFX_DrawString(5, 5, "PAUSED WORK REMAINED", PIXEL_WHITE, 0);
			}
			else
			{
				GFX_DrawString(5, 5, "PAUSED RELAX REMAINED", PIXEL_WHITE, 0);
			}

			//draw remaining time
			ChangeTimeFormat(Pomodoro.TimeToDisplay, &Hours, &Minutes, &Seconds);
			GFX_DrawString(10, 20, TimeBuffer, PIXEL_WHITE, 0);

			GFX_DrawString(10, 47, "CLICK TO RESUME", PIXEL_WHITE, 0);
			SSD1306_Display();
		break;
	case POMO_STATE_ALARM:		//draw alarm
			SSD1306_Clear(BLACK);

			GFX_DrawString(10, 5, "ALARM", PIXEL_WHITE, 0);

			if  (Pomodoro.CurrentPhase == POMO_PHASE_RELAX)
			{
				GFX_DrawString(5, 17, "Get ready to work!", PIXEL_WHITE, 0);
			}
			else
			{
				GFX_DrawString(5, 17, "Get ready to relax!", PIXEL_WHITE, 0);
			}

			SSD1306_Display();
		break;
	case POMO_STATE_EDIT:		//draw edit
			if (Pomodoro.EditTarget == POMO_EDIT_WORK)
			{
				SSD1306_Clear(BLACK);
				//GFX_DrawRectangle(0, 0, 128, 63, PIXEL_WHITE);;
				GFX_SetFont(font_8x5);

				uint8_t Length = 0;

				Minutes = Pomodoro.CfgWorkTime % 60;
				Hours = Pomodoro.CfgWorkTime / 60;
				sprintf(TimeBuffer, "%02d:%02d", Hours, Minutes);
				Length = sprintf(Buffer, "WORK TIME: %s", TimeBuffer);
				GFX_DrawFillRectangle(5 - 2 , 5 - 2, Length * 6 + 4, 8 + 4, PIXEL_WHITE);
				GFX_DrawString(5, 5, Buffer, PIXEL_BLACK, 1);


				Minutes = Pomodoro.CfgRelaxTime % 60;
				Hours = Pomodoro.CfgRelaxTime / 60;
				sprintf(TimeBuffer, "%02d:%02d", Hours, Minutes);
				Length = sprintf(Buffer, "RELAX TIME: %s", TimeBuffer);
				GFX_DrawString(5, 17, Buffer, PIXEL_WHITE, 0);
				//GFX_DrawString(5, 17, "RELAX TIME: %s", TimeBuffer, PIXEL_WHITE, 0);


				SSD1306_Display();
			}
			else
			{
				SSD1306_Clear(BLACK);
				//GFX_DrawRectangle(0, 0, 128, 63, PIXEL_WHITE);
				GFX_SetFont(font_8x5);
				//DrawMenuItem(5, "WORK TIME:", Pomodoro.CfgWorkTime, 0);
				//DrawMenuItem(17, "RELAX TIME:", Pomodoro.CfgRelaxTime, 1);
				uint8_t Length = 0;

				Minutes = Pomodoro.CfgWorkTime % 60;
				Hours = Pomodoro.CfgWorkTime / 60;
				sprintf(TimeBuffer, "%02d:%02d", Hours, Minutes);
				Length = sprintf(Buffer, "WORK TIME: %s", TimeBuffer);
				GFX_DrawString(5, 5, Buffer, PIXEL_WHITE, 0);

				Minutes = Pomodoro.CfgRelaxTime % 60;
				Hours = Pomodoro.CfgRelaxTime / 60;
				sprintf(TimeBuffer, "%02d:%02d", Hours, Minutes);
				Length = sprintf(Buffer, "RELAX TIME: %s", TimeBuffer);
				GFX_DrawFillRectangle(5 - 2 , 17 - 2, Length * 6 + 4, 8 + 4, PIXEL_WHITE);
				GFX_DrawString(5, 17, Buffer, PIXEL_BLACK, 1);

				SSD1306_Display();
			}
		break;
	}
}
void ChangeTimeFormat(int32_t TimeInSeconds, uint8_t *Hours, uint8_t *Minutes, uint8_t *Seconds)
{
	*Seconds = (uint8_t) (TimeInSeconds % 60);

	*Minutes = (uint8_t) ((TimeInSeconds / 60) % 60);

	*Hours = (uint8_t) (TimeInSeconds / 3600);
	if(*Hours > 0)
		sprintf(TimeBuffer, "%02d:%02d:%02d", *Hours, *Minutes, *Seconds);
	else
		sprintf(TimeBuffer, "%02d:%02d", *Minutes, *Seconds);
}

void DrawMenuItem(uint8_t y, char* Label, uint16_t Value, uint8_t IsSelected)
{
	char TempBuffer[32];
	uint8_t Length;
	uint8_t x = 10;

	Length = sprintf(TempBuffer, "%s: %d", Label, Value);

	if(IsSelected)
	{
		GFX_DrawFillRectangle(x - 2 , y - 2, Length * 6 + 4, 8 + 4, PIXEL_WHITE);
		GFX_DrawString(x, y, TempBuffer, PIXEL_BLACK, 1);	//black text
	}
	else
	{
		GFX_DrawString(x, y, TempBuffer, PIXEL_WHITE, 0);	//white text
	}
}
/*
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
*/

/*
void UpdateDateWithSeconds(RTC_TimeTypeDef *Time, uint32_t SecondsToAdd)
{
	Time->Seconds = SecondsToAdd % 60;

	uint32_t MinutesToAdd = SecondsToAdd / 60;
	Time->Minutes = MinutesToAdd % 60;

	uint32_t HoursToAdd = MinutesToAdd / 24;
	Time->Hours = HoursToAdd % 24;
}
*/

/*
void TimeToSeconds(RTC_TimeTypeDef Time, uint32_t *Seconds)
{
	Seconds = Time.Seconds + Time.Minutes * 60 + Time.Hours * 3600;
}
*/
void ButtonMidPress()
{
	TurnOnLed();
	Pomodoro.Event = POMO_EVENT_ACTION;
}
void ButtonMidLongPress()
{
	TurnOffLed();
	Pomodoro.Event = POMO_EVENT_ACTION_2;
}
void ButtonTopPress()
{
	TurnOnLed();
	Pomodoro.Event = POMO_EVENT_INC;
	Pomodoro.EventParam = 1;
}
void ButtonTopLongPress()
{
	TurnOffLed();
	Pomodoro.Event = POMO_EVENT_INC;
	Pomodoro.EventParam = 4;
}
void ButtonTopRepeat()
{
	ToggleLed();
	Pomodoro.Event = POMO_EVENT_INC;
	Pomodoro.EventParam = 5;
}
void ButtonBottomPress()
{
	TurnOnLed();
	Pomodoro.Event = POMO_EVENT_INC;
	Pomodoro.EventParam = -1;
}
void ButtonBottomLongPress()
{
	TurnOffLed();
	Pomodoro.Event = POMO_EVENT_INC;
	Pomodoro.EventParam = -4;
}
void ButtonBottomRepeat()
{
	ToggleLed();
	Pomodoro.Event = POMO_EVENT_INC;
	Pomodoro.EventParam = -5;
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

//calculate unix time (seconds passed from 1900)
int32_t GetCurrentUnixTime(RTC_HandleTypeDef *hrtc)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    struct tm timeStruct = {0};

    // WAŻNE: W STM32 musisz odczytać najpierw CZAS, potem DATĘ!
    HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN);

    timeStruct.tm_year = sDate.Year + 100; // od 1900 roku
    timeStruct.tm_mon  = sDate.Month - 1;  // 0-11
    timeStruct.tm_mday = sDate.Date;
    timeStruct.tm_hour = sTime.Hours;
    timeStruct.tm_min  = sTime.Minutes;
    timeStruct.tm_sec  = sTime.Seconds;
    timeStruct.tm_isdst = -1;

    return (int32_t)mktime(&timeStruct);
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
