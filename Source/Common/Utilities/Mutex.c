/*
 * Semaphore.c
 *
 *  Created on: 2016年1月13日
 *      Author: seeing
 */

#include "Mutex.h"

SeMutexType SeMutexCreate(void)
{
#ifdef CONFIG_USING_FREERTOS
	SeMutexType tMutex;
	tMutex = xSemaphoreCreateMutex();
	return tMutex;
#endif

}

void SeMutexLock(SeMutexType tMutex)
{
#ifdef CONFIG_USING_FREERTOS
	xSemaphoreTake(tMutex, portMAX_DELAY);
#endif
}

void SeMutexUnlock(SeMutexType tMutex)
{
#ifdef CONFIG_USING_FREERTOS
	xSemaphoreGive(tMutex);
#endif
}

void SeMutexDestroy(SeMutexType tMutex)
{
#ifdef CONFIG_USING_FREERTOS
	vSemaphoreDelete(tMutex);
#endif
}
