/*
 * Timer.h
 *
 *  Created on: 2016年1月12日
 *      Author: seeing
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include "../../Global/Include.h"

#ifndef CONFIG_MAX_TIMERS
#define MAX_TIMERS 2
#else
#define MAX_TIMERS CONFIG_MAX_TIMERS
#endif

typedef struct {
	void (*fpSeTimerCallback)(void);
}SeTimerCallback;

typedef struct {
	SeTimeValue tInterval;
	SeTimerCallback tCallback;
	SeInt8 (*fpSeTimerPreInit)(SeTimeValue tInterval, SeTimerCallback tCallback);
	void (*fpSeTimerStart)(void);
	void (*fpSeTimerStop)(void);
	SeBool blEnable;
}SeTimer;

#ifdef __cplusplus
extern "C" {
#endif

SeTimeValue SeGetCurrentTimeValue(void);

void SeAdjustTimeValue(SeTimeValue tTimeValue);

void SeTimerIncreaseAMilliSecond(void);

void SeTimerIncreaseAMicroSecond(void);

void SeTimerIncreaseASecond(void);

SeInt8 SeTimerInit(SeInt8 iIndex, SeTimer tTimer);
void SeTimerStart(SeInt8 iIndex);
void SeTimerStop(SeInt8 iIndex);

#ifdef __cplusplus
}
#endif

#endif /* _TIMER_H_ */
