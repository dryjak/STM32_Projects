/*
 * PomodoroFSM.c
 *
 *  Created on: Feb 6, 2026
 *      Author: dryla
 */
#include <PomodoroFSM.h>




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
	}
}
