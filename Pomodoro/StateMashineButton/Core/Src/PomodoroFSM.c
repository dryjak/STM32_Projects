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

	Pomodoro->TargetTimestamp	= 0;
	Pomodoro->SavedTimeLeft 	= 0;

	Pomodoro->TimeToDisplay 	= 0;
	Pomodoro->TriggerAlarm 		= 0;
	Pomodoro->NeedsRedraw		= 1;
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
