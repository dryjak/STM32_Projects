/*
 * PomodoroFSM.c
 *
 *  Created on: Feb 6, 2026
 *      Author: dryla
 */
#include <PomodoroFSM.h>

#define MAX_EVENT_TIME 99
#define MIN_EVENT_TIME 1

//private function prototypes
static void UpdateDisplayTime(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime);
static void PomodoroStateRunning(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime);
static void PomodoroStatePause(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime);
static void PomodoroStateAlarm(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime);
static void PomodoroStateEdit(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime);


void PomodoroInit(Pomodoro_t *Pomodoro)
{
	Pomodoro->CfgWorkTime	= 25;
	Pomodoro->CfgRelaxTime 	= 5;
	Pomodoro->EventParam 	= 1;

	Pomodoro->CurrentState	= POMO_STATE_IDLE;
	Pomodoro->CurrentPhase 	= POMO_PHASE_WORK;
	Pomodoro->EditTarget 	= POMO_EDIT_WORK;

	Pomodoro->TargetTimeStamp	= 0;
	Pomodoro->SavedTimeLeft   	= Pomodoro->CfgWorkTime * 60; // Na start pokaż pełny czas
	Pomodoro->TimeToDisplay   	= Pomodoro->SavedTimeLeft;
	Pomodoro->Event 			= POMO_EVENT_NONE;

	Pomodoro->NeedsRedraw		= 1;
	Pomodoro->TriggerAlarm 		= 0;
}

static void UpdateDisplayTime(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime)
{
    if (Pomodoro->CurrentState == POMO_STATE_RUNNING)
    {
        int32_t Remaining = Pomodoro->TargetTimeStamp - CurrentUnixTime;
        if (Remaining < 0) Remaining = 0;
        Pomodoro->TimeToDisplay = Remaining;
    }
    else
    {
        // W IDLE i PAUSED wyświetlamy to co zapamiętane/skonfigurowane
        Pomodoro->TimeToDisplay = Pomodoro->SavedTimeLeft;
    }
}

//Do Idle
static void PomodoroStateIdle(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime)
{
	//Check if there was input
	if(Pomodoro->Event == POMO_EVENT_ACTION)
	{
		//START
		Pomodoro->CurrentState = POMO_STATE_RUNNING;
		//calculate time
		int32_t Duration = 0;
		if(Pomodoro->CurrentPhase == POMO_PHASE_WORK)
		{
			Duration = Pomodoro->CfgWorkTime * 60;
		}
		else if(Pomodoro->CurrentPhase == POMO_PHASE_RELAX)
		{
			Duration = Pomodoro->CfgRelaxTime * 60;

		}
		Pomodoro->TargetTimeStamp = CurrentUnixTime + Duration;
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

static void PomodoroStateRunning(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime)
{
	if(Pomodoro->Event == POMO_EVENT_ACTION)
	{
		//PAUSE
		Pomodoro->CurrentState = POMO_STATE_PAUSED;
		//Remember time left
		Pomodoro->SavedTimeLeft = Pomodoro->TargetTimeStamp - CurrentUnixTime;
		Pomodoro->NeedsRedraw = 1;
	}
	if(CurrentUnixTime >= Pomodoro->TargetTimeStamp)
	{
		Pomodoro->CurrentState = POMO_STATE_ALARM;
		Pomodoro->NeedsRedraw = 1;
		Pomodoro->TriggerAlarm = 1;
	}
	if(Pomodoro->Event == POMO_EVENT_ACTION_2)
	{
		//go to edit mode
		Pomodoro->CurrentState = POMO_STATE_EDIT;
		Pomodoro->EditTarget = POMO_EDIT_WORK;
		Pomodoro->NeedsRedraw = 1;
	}
}

static void PomodoroStatePause(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime)
{
	if(Pomodoro->Event == POMO_EVENT_ACTION)
	{
		//RESUME
		Pomodoro->CurrentState = POMO_STATE_RUNNING;
		Pomodoro->TargetTimeStamp = CurrentUnixTime + Pomodoro->SavedTimeLeft;
		Pomodoro->NeedsRedraw = 1;
	}
	if (Pomodoro->Event == POMO_EVENT_ACTION_2)
	{
		//go to edit mode
		Pomodoro->CurrentState = POMO_STATE_EDIT;
		Pomodoro->EditTarget = POMO_EDIT_WORK;
		Pomodoro->NeedsRedraw = 1;
	}
}
static void PomodoroStateAlarm(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime)
{
	if(Pomodoro->Event == POMO_EVENT_ACTION)
	{
		Pomodoro->CurrentState = POMO_STATE_IDLE;
		Pomodoro->TriggerAlarm = 0;

		if(Pomodoro->CurrentPhase == POMO_PHASE_WORK)
		{
			Pomodoro->CurrentPhase = POMO_PHASE_RELAX;
			Pomodoro->SavedTimeLeft = Pomodoro->CfgRelaxTime * 60;
			//Pomodoro->TargetTimeStamp = CurrentUnixTime + Pomodoro->CfgRelaxTime;
		}
		else if(Pomodoro->CurrentPhase == POMO_PHASE_RELAX)
		{
			Pomodoro->CurrentPhase = POMO_PHASE_WORK;
			Pomodoro->SavedTimeLeft = Pomodoro->CfgRelaxTime * 60;
			//Pomodoro->TargetTimeStamp = CurrentUnixTime + Pomodoro->CfgWorkTime;
		}
		if(Pomodoro->CurrentPhase == POMO_PHASE_RELAX)	//after finishing work go straight to relax (without idle state)
		{
			Pomodoro->CurrentState = POMO_STATE_RUNNING;
			int32_t Duration = 0;
			Duration = Pomodoro->CfgRelaxTime * 60;
			Pomodoro->TargetTimeStamp = CurrentUnixTime + Duration;
		}
		Pomodoro->NeedsRedraw = 1;
	}
}
static void PomodoroStateEdit(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime)
{
	if(Pomodoro->Event == POMO_EVENT_ACTION)
	{
		//change edited column
		if(Pomodoro->EditTarget == POMO_EDIT_WORK)
		{
			Pomodoro->EditTarget = POMO_EDIT_RELAX;
		}
		else
		{
			Pomodoro->EditTarget = POMO_EDIT_WORK;
		}
		Pomodoro->NeedsRedraw = 1;
	}
	//go to idle
	if(Pomodoro->Event == POMO_EVENT_ACTION_2)
	{
		Pomodoro->CurrentState = POMO_STATE_IDLE;

		if(Pomodoro->CurrentPhase == POMO_PHASE_WORK)
			Pomodoro->SavedTimeLeft = Pomodoro->CfgWorkTime * 60;
		else
			Pomodoro->SavedTimeLeft = Pomodoro->CfgRelaxTime * 60;

		Pomodoro->NeedsRedraw = 1;
	}
	if(Pomodoro->Event == POMO_EVENT_INC)
	{
		//increase value
		if(Pomodoro->EditTarget == POMO_EDIT_WORK)
		{
			ModifyConfigValue(&Pomodoro->CfgWorkTime, Pomodoro->EventParam);
		}
		else if(Pomodoro->EditTarget == POMO_EDIT_RELAX)
		{
			ModifyConfigValue(&Pomodoro->CfgRelaxTime, Pomodoro->EventParam);
		}
		Pomodoro->NeedsRedraw = 1;
	}
	if(Pomodoro->Event == POMO_EVENT_DEC)
	{
		//decline value (Pomodoro->EventParam can be < 0)
		if(Pomodoro->EditTarget == POMO_EDIT_WORK)
		{
			ModifyConfigValue(&Pomodoro->CfgWorkTime, Pomodoro->EventParam);
		}
		else if(Pomodoro->EditTarget == POMO_EDIT_RELAX)
		{
			ModifyConfigValue(&Pomodoro->CfgRelaxTime, Pomodoro->EventParam);
		}
		Pomodoro->NeedsRedraw = 1;
	}
}

void ModifyConfigValue(uint16_t *ValueToModify, int8_t ValueThatModifies)
{
	int32_t Tmp = *ValueToModify + ValueThatModifies;

	if(Tmp < MIN_EVENT_TIME) Tmp = MIN_EVENT_TIME;
	else if(Tmp > MAX_EVENT_TIME) Tmp = MAX_EVENT_TIME;

	*ValueToModify = (uint16_t)Tmp;
}


void PomodoroTask(Pomodoro_t *Pomodoro, int32_t CurrentTime)
{
	switch (Pomodoro->CurrentState)
	{
	case POMO_STATE_IDLE:
		//do idle
		PomodoroStateIdle(Pomodoro, CurrentTime);
		break;
	case POMO_STATE_RUNNING:
		//do running
		PomodoroStateRunning(Pomodoro, CurrentTime);
		break;
	case POMO_STATE_PAUSED:
		//do pause
		PomodoroStatePause(Pomodoro, CurrentTime);
		break;
	case POMO_STATE_ALARM:
		//do alarm
		PomodoroStateAlarm(Pomodoro, CurrentTime);
		break;
	case POMO_STATE_EDIT:
		//do edit
		PomodoroStateEdit(Pomodoro, CurrentTime);
		break;
	}
	Pomodoro->Event = POMO_EVENT_NONE;

	//update time to display
	int32_t lastTime = Pomodoro->TimeToDisplay;
	UpdateDisplayTime(Pomodoro, CurrentTime);

	if (lastTime != Pomodoro->TimeToDisplay)
	{
		Pomodoro->NeedsRedraw = 1;
	}
}
