/*
 * Semaphore.h
 *
 *  Created on: 2016年1月13日
 *      Author: seeing
 */

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_USING_FREERTOS
#include "../System/System.h"
#endif

#ifdef CONFIG_USING_LINUX
#include <pthread.h>
#include <semaphore.h>
#endif

/*
 * define semaphore type
 */
#ifdef CONFIG_USING_FREERTOS
typedef xSemaphoreHandle SeMutexType;
#endif

#ifdef CONFIG_USING_LINUX

#endif

SeMutexType SeMutexCreate(void);

void SeMutexLock(SeMutexType tMutex);

void SeMutexUnlock(SeMutexType tMutex);

void SeMutexDestroy(SeMutexType tMutex);

#ifdef __cplusplus
}
#endif

#endif /* _SEMAPHORE_H_ */
