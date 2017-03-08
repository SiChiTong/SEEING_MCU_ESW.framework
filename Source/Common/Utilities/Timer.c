/*
 * Timer.c
 *
 *  Created on: 2016年1月12日
 *      Author: seeing
 */


#include "Timer.h"

static SeTimeValue tGlobalTimeValue;
static SeTimer aGlobalTimers[MAX_TIMERS] = {0};

SeTimeValue SeGetCurrentTimeValue(void)
{
	return tGlobalTimeValue;
}

void SeAdjustTimeValue(SeTimeValue tTimeValue)
{
	tGlobalTimeValue = tTimeValue;
}

void SeTimerIncreaseAMilliSecond(void)
{
	tGlobalTimeValue.iMicroSeconds += 1000;
	if(tGlobalTimeValue.iMicroSeconds == 1000000)
	{
		tGlobalTimeValue.iMicroSeconds = 0;
		tGlobalTimeValue.iUtcSeconds++;
	}
}

void SeTimerIncreaseAMicroSecond(void)
{
	tGlobalTimeValue.iMicroSeconds++;
	if(tGlobalTimeValue.iMicroSeconds == 1000000)
	{
		tGlobalTimeValue.iMicroSeconds = 0;
		tGlobalTimeValue.iUtcSeconds++;
	}
}

void SeTimerIncreaseASecond(void)
{
	tGlobalTimeValue.iUtcSeconds++;
}

SeInt8 SeTimerInit(SeInt8 iIndex, SeTimer tTimer)
{
	if(iIndex < 0 || iIndex >= MAX_TIMERS)
	{
		return SE_RETURN_ERROR;
	}

	if(tTimer.fpSeTimerPreInit(tTimer.tInterval, tTimer.tCallback) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	aGlobalTimers[iIndex] = tTimer;

	SeDebugPrint("Create timer as %lld seconds and %lld microseconds.", tTimer.tInterval.iUtcSeconds, tTimer.tInterval.iMicroSeconds);

	return SE_RETURN_OK;
}

void SeTimerStart(SeInt8 iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_TIMERS)
	{
		return;
	}

	if(aGlobalTimers[iIndex].blEnable)
		return;

	aGlobalTimers[iIndex].fpSeTimerStart();
	aGlobalTimers[iIndex].blEnable = SeTrue;
}

void SeTimerStop(SeInt8 iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_TIMERS)
	{
		return;
	}

	if(!aGlobalTimers[iIndex].blEnable)
		return;

	aGlobalTimers[iIndex].fpSeTimerStop();
	aGlobalTimers[iIndex].blEnable = SeFalse;
}
