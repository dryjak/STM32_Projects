/*
 * PomodoroFSM.h
 *
 *  Created on: Feb 6, 2026
 *      Author: dryla
 */

#ifndef INC_POMODOROFSM_H_
#define INC_POMODOROFSM_H_

#include <stdint.h>
#include <stdio.h>
#include <rtc.h>

typedef enum{
	POMO_EVENT_NONE = 0,

	POMO_EVENT_ACTION,
	POMO_EVENT_ACTION_2,

	POMO_EVENT_INC,
	POMO_EVENT_DEC
}PomoEvent_t;

typedef enum{
	POMO_STATE_IDLE = 0,
	POMO_STATE_RUNNING,
	POMO_STATE_PAUSED,
	POMO_STATE_ALARM,
	POMO_STATE_EDIT
}PomoState_t;

typedef enum{
	POMO_EDIT_WORK = 0,
	POMO_EDIT_RELAX
}PomoEditTarget_t;

typedef enum{
    POMO_PHASE_WORK = 0,
    POMO_PHASE_RELAX
} PomoPhase_t;

typedef struct{
	uint16_t CfgWorkTime;	//Configured time to work in minutes
	uint16_t CfgRelaxTime;	//Configured time to relax in minutes
	int8_t EventParam;

	PomoState_t 		CurrentState;	//IDLE, RUNNING, PAUSED ...
	PomoPhase_t         CurrentPhase;   //(Work/Relax)
	PomoEditTarget_t	EditTarget;		//what do we edit Worktime or Relaxtime
	PomoEvent_t 		Event;			//variable value is based on inputs

	int32_t TargetTimeStamp;		//when is the end
	int32_t TimeToDisplay;			//whoale time in seconds to display
	int32_t SavedTimeLeft;			//time left

//	int8_t TimeToDisplayHours;
//	int8_t TimeToDisplayMinutes;
//	int8_t TimeToDisplaySeconds;

	uint8_t NeedsRedraw;	//Flag: 1 - display needs redraw
	uint8_t TriggerAlarm;	//Flag: 1 - trigger alarm
}Pomodoro_t;



void PomodoroInit(Pomodoro_t *Pomodoro);
void PomodoroTask(Pomodoro_t *Pomodoro, int32_t CurrentUnixTime);

//function that modifies value e.g. CfgWorkTime or CfgRelaxTime
void ModifyConfigValue(uint16_t *ValueToModify, int8_t ValueThatModifies);



#endif /* INC_POMODOROFSM_H_ */





