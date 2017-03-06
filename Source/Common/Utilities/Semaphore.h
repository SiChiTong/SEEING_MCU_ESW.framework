/*
 * Semaphore.h
 *
 *  Created on: 2016年1月13日
 *      Author: seeing
 */

#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

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
#define SE_SEMAPHORE_INFINITE_WAIT			0
typedef xSemaphoreHandle SeSemaphoreType;
#endif

#ifdef CONFIG_USING_LINUX
#define SE_SEMAPHORE_INFINITE_WAIT			0
typedef sem_t SeSemaphoreType;
#endif

void SeSemaphoreCreate(SeSemaphoreType* pSemaphore);

void SeSemaphorePost(SeSemaphoreType tSemaphore);

void SeSemaphoreWait(SeSemaphoreType tSemaphore, SeUInt32 nWaitTime);

void SeSemaphoreDestroy(SeSemaphoreType tSemaphore);

#ifdef __cplusplus
}
#endif

#endif /* _SEMAPHORE_H_ */
