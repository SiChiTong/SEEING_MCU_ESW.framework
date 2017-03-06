/*
 * Delay.c
 *
 *  Created on: 2015-11-19
 *      Author: Administrator
 */


#include "Delay.h"

static SeDelayOperation tGlobalDelayOperation;

SeInt8 SeDelayInit(SeDelayOperation tDelayOper)
{

	return SE_RETURN_OK;
}



void SeDelayMs(SeUInt32 nMs)
{
	SeUInt32 iCount;
#ifdef CONFIG_USING_FREERTOS
	for(iCount = 0; iCount < nMs; iCount++)
	{
		vTaskDelay(CONFIG_FREERTOS_TICKS_PER_MILLISECONDS);
	}
#endif
}

void SeDelayCycles(SeUInt32 nCycles)
{
	SeUInt32 iCount;
	for(iCount = 0; iCount < nCycles; iCount++);
}
