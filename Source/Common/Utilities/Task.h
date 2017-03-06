/*
 * Task.h
 *
 *  Created on: 2016年1月13日
 *      Author: seeing
 */

#ifndef _TASK_H_
#define _TASK_H_


#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TASK_COUNT	16

#ifdef CONFIG_USING_FREERTOS
#include "../System/System.h"
#endif

#ifdef CONFIG_USING_LINUX
#include <pthread.h>
#include <semaphore.h>
#endif

#ifdef CONFIG_USING_FREERTOS
typedef xTaskHandle SeTaskHandleType;
typedef void SeTaskReturnType;
#endif

#ifdef CONFIG_USING_LINUX
typedef pthread_t SeTaskHandleType;
typedef void* SeTaskReturnType;
#endif

typedef enum
{
	SeTaskPriorityIdle = 0,
	SeTaskPriorityLow,
	SeTaskPriorityNormal,
	SeTaskPriorityHigher,
	SeTaskPriorityHighest
}SeTaskPriority;

SeInt8 SeTaskAdd(SeInt8* iIndex, void* fpTaskRoutine, void* pArgument, SeTaskPriority tTaskPriority, SeUInt32 nTaskStackSize);

SeInt8 SeTaskStart(SeInt8* iIndex, void* fpTaskRoutine, void* pArgument, SeTaskPriority tTaskPriority, SeUInt32 nTaskStackSize);

SeInt8 SeTaskRemove(SeInt8 iIndex);

SeInt8 SeTaskSelfRemove(void);

SeInt8 SeTaskSchedule(void);

#ifdef __cplusplus
}
#endif

#endif /* _TASK_H_ */
