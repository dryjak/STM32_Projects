/*
 * PomodoroFSM.c
 *
 *  Created on: Feb 6, 2026
 *      Author: dryla
 */
#include <PomodoroFSM.h>


void PomodoroInit(Pomodoro_t *Pomodoro)
{
	Pomodoro->CfgWorkTime = 25;
	Pomodoro->CfgRelaxTie = 5;

	Pomodoro->CurrentState	= POMO_STATE_IDLE;
	Pomodoro->CurrentMode 	= POMO_MODE_NORMAL;
	Pomodoro->EditTarget 	= POMO_EDIT_WORK;

	Pomodoro->TargetTimeStamp	= 0;
	Pomodoro->SavedTimeLeft 	= 0;
	Pomodoro->Event 			= POMO_EVENT_IDLE;

	Pomodoro->NeedsRedraw 	= 0;
	Pomodoro->TriggerAlarm 		= 0;
	Pomodoro->NeedsRedraw		= 0;
}

int32_t ChangeTimeToSeconds(RTC_TimeTypeDef Time)
{
	int32_t TimeInSeconds = 0;
	TimeInSeconds  = Time.Seconds + Time.Minutes * 60 + Time.Hours * 3600;
	return  TimeInSeconds;
}

void CalculateFormatDisplayTime(Pomodoro_t *Pomodoro)
{
	Pomodoro->TimeToDisplayHours = 0;
	Pomodoro->TimeToDisplayMinutes = 0;
	Pomodoro->TimeToDisplaySeconds = 0;

	int32_t TmpTime;
	Pomodoro->TimeToDisplaySeconds = (Pomodoro->SavedTimeLeft) % 60;

	TmpTime = (Pomodoro->SavedTimeLeft) / 60;
	Pomodoro->TimeToDisplayMinutes = TmpTime % 60;

	TmpTime = (Pomodoro->SavedTimeLeft) / 60;
	Pomodoro->TimeToDisplayHours = TmpTime % 24;
}

//Do Idle
void PomodoroStateIdle(Pomodoro_t *Pomodoro)
{
	//Check if there was input
	if(Pomodoro->Event == POMO_EVENT_ACTION)
	{
		//go to Running
		Pomodoro->CurrentState = POMO_STATE_RUNNING;
		//calculate end time in seconds
		Pomodoro->TargetTimeStamp = Pomodoro->TimeBeginInSeconds + Pomodoro->CfgWorkTime * 60;
		CalculateFormatDisplayTime(Pomodoro);
		//calculate end time in seconds

		 Pomodoro->NeedsRedraw = 1;

	}
	else if(Pomodoro->Event == POMO_EVENT_ACTION_2)
	{
		//go to edit mode
		Pomodoro->CurrentState = POMO_STATE_EDIT;
		Pomodoro->EditTarget = POMO_EDIT_WORK;
		Pomodoro->NeedsRedraw = 1;
	}
}

void PomodoroStateRunning(Pomodoro_t *Pomodoro)
{
	if(Pomodoro->Event == POMO_EVENT_ACTION)
	{
		//Go to paused state
	}
	else if (Pomodoro->Event == POMO_EVENT_ACTION_2)
	{
		//go to edit mode
		Pomodoro->CurrentState = POMO_STATE_EDIT;
		Pomodoro->EditTarget = POMO_EDIT_WORK;
		Pomodoro->NeedsRedraw = 1;
	}
}


void PomodoroTask(Pomodoro_t *Pomodoro, int32_t CurrentTime)
{
	switch (Pomodoro->CurrentState)
	{
	case POMO_STATE_IDLE:
		//do idle
		break;
	case POMO_STATE_RUNNING:
		//do running
		break;
	case POMO_STATE_PAUSED:
		//do pause
		break;
	case POMO_STATE_ALARM:
		//do alarm
		break;
	case POMO_STATE_EDIT:
		//do edit
		break;
	}
}
