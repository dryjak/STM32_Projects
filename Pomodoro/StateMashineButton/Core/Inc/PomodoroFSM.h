/*
 * PomodoroFSM.h
 *
 *  Created on: Feb 6, 2026
 *      Author: dryla
 */

#ifndef INC_POMODOROFSM_H_
#define INC_POMODOROFSM_H_

#include <main.h>

typedef enum{
	POMO_EVENT_IDLE = 0,

	POMO_EVENT_ACTION,
	POMO_EVENT_MODE_TOGGLE,

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
	POMO_MODE_NORMAL = 0,
	POMO_MODE_EDIT
}PomoMode_t;

typedef enum{
	POMO_EDIT_WORK = 0,
	POMO_EDIT_RELAX
}PomoEditTarget_t;

typedef struct{
	uint16_t CfgWorkTime;	//Configured time to work
	uint16_t CfgRelaxTie;	//Configured time to relax

	PomoState_t 		CurrentState;	//IDLE, RUNNING, PAUSED ...
	PomoMode_t 			CurrentPhase;	//Work or relax
	PomoEditTarget_t	EditTarget;		//what do we edit Worktime or Relaxtime

	int32_t TargetTimestamp;
	int32_t SavedTimeLeft;

	int32_t TimeToDisplay;	//Left seconds to display
	uint8_t NeedsRedraw;	//Flag: 1 - display needs redraw
	uint8_t TriggerAlarm;	//Flag: 1 - trigger alarm
}Pomodoro_t;



#endif /* INC_POMODOROFSM_H_ */

void PomodoroInit(Pomodoro_t *Pomodoro);



