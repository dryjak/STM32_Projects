/*
 * PomodoroFSM.h
 *
 *  Created on: Feb 6, 2026
 *      Author: dryla
 */

#ifndef INC_POMODOROFSM_H_
#define INC_POMODOROFSM_H_


//State definiction
typedef enum{
	NORMAL_MODE = 0,
	EDIT_MODE
}SystemMode_t;

typedef enum {
    POMO_PHASE_WORK = 0,
    POMO_PHASE_RELAX
} PomoPhase_t;

typedef enum {
    POMO_STATE_IDLE = 0,
    POMO_STATE_RUNNING,
    POMO_STATE_PAUSED,
    POMO_STATE_ALARM
} PomoState_t;


typedef struct {
    // Config
    int16_t ConfigWorkMin;
    int16_t ConfigRelaxMin;
    int16_t ConfigWorkMax;
    int16_t ConfigWorkMax;

    int16_t WorkTime;
    int16_t RelaxTime;

    // Current phase - work / relax, CurrentState - idle / running / paused / alarm
    PomoPhase_t CurrentPhase;
    PomoState_t CurrentState;

    time_t TargetTimestamp; //when is the finish
    int32_t TimeLeftStored; //remembered time stored

    //variables for display
    int32_t TimeToDisplay;
    uint8_t DispalyNeedsUpdate;    //refresh display
    uint8_t TriggerAlarm;   //enable alarm
} Pomodoro_t;
#endif /* INC_POMODOROFSM_H_ */
