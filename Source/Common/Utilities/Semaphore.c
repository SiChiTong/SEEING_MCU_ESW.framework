/*
 * Semaphore.c
 *
 *  Created on: 2016年1月13日
 *      Author: seeing
 */

#include "Semaphore.h"

void SeSemaphoreCreate(SeSemaphoreType* pSemaphore)
{
#ifdef CONFIG_USING_FREERTOS
	vSemaphoreCreateBinary(*pSemaphore);
	/*
	 * In FreeRtos V7,after semaphore created,the status was '1',must give '0' to it to be block
	 */
	xSemaphoreTake(*pSemaphore, 1);
#endif

}

void SeSemaphorePost(SeSemaphoreType tSemaphore)
{
#ifdef CONFIG_USING_FREERTOS
	xSemaphoreGive(tSemaphore);
#endif
}

void SeSemaphoreWait(SeSemaphoreType tSemaphore, SeUInt32 nWaitTime)
{
#ifdef CONFIG_USING_FREERTOS
	if(nWaitTime == SE_SEMAPHORE_INFINITE_WAIT)
	{
		while(xSemaphoreTake(tSemaphore, portMAX_DELAY) != pdTRUE){}
	}else{
		xSemaphoreTake(tSemaphore, nWaitTime);
	}
#endif
}

void SeSemaphoreDestroy(SeSemaphoreType tSemaphore)
{
#ifdef CONFIG_USING_FREERTOS
	vSemaphoreDelete(tSemaphore);
#endif
}
