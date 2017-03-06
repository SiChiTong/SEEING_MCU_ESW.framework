/*
 * Task.c
 *
 *  Created on: 2016年1月13日
 *      Author: seeing
 */

#include "Task.h"

typedef enum
{
	SeTaskStatusTypeInvalid = 0,
	SeTaskStatusTypeAdded,
	SeTaskStatusTypeRunning,
}SeTaskStatusType;

typedef struct
{
	SeTaskStatusType tTaskStatus;
	SeTaskHandleType tTaskHandle;
	SeTaskPriority tPriority;
	SeUInt32 nStackSize;
	void* fpRoutine;
	void* pArgument;
#ifdef CONFIG_USING_FREERTOS
	SeUInt8 nFreeRtosPriority;
#endif
}SeTaskDescription;

static SeTaskDescription aGlobalTasksTable[CONFIG_MAX_TASKS];

#define TASK_IS_INFINITE 					-1

static SeSemaphoreType tTaskManagerSemaphore;

static SeTaskReturnType SeTaskManagerRoutine(void* pArgument)
{
	SeInt8 iCount;

#ifdef CONFIG_USING_FREERTOS
	SeUInt8 nFreeRtosLowestPriority = tskIDLE_PRIORITY + 1;
	SeUInt8 nFreeRtosHighPriorityThreshold = configMAX_PRIORITIES - 4;
	SeUInt8 nFreeRtosHighestPriority = nFreeRtosHighPriorityThreshold;
	SeUInt8 nFreeRtosNormalHighestPriority = nFreeRtosLowestPriority + 1;
#endif

	SeSemaphorePost(tTaskManagerSemaphore);
	SeDebugPrint("Task manager processing...");

	while(SeTrue)
	{
		SeSemaphoreWait(tTaskManagerSemaphore, SE_SEMAPHORE_INFINITE_WAIT);

		for(iCount = 0; iCount < CONFIG_MAX_TASKS; iCount++)
		{
			if(aGlobalTasksTable[iCount].tTaskStatus == SeTaskStatusTypeAdded)
			{
#ifdef CONFIG_USING_FREERTOS
				SeUInt8 nPriority = tskIDLE_PRIORITY + 2;

				switch(aGlobalTasksTable[iCount].tPriority)
				{
					case SeTaskPriorityLow:
						nPriority = tskIDLE_PRIORITY + 1;
						break;
					case SeTaskPriorityNormal:
						if(nFreeRtosNormalHighestPriority < (nFreeRtosHighPriorityThreshold - 1))
						{
							nFreeRtosNormalHighestPriority++;
						}
						nPriority = nFreeRtosNormalHighestPriority;
						break;
					case SeTaskPriorityHigher:
						if(nFreeRtosHighestPriority < (configMAX_PRIORITIES - 2))
						{
							nFreeRtosHighestPriority++;
						}
						nPriority = nFreeRtosHighestPriority;
						break;
					case SeTaskPriorityHighest:
						nPriority = configMAX_PRIORITIES - 1;
						break;
					default:break;
				}


				if(xTaskCreate(aGlobalTasksTable[iCount].fpRoutine, "", aGlobalTasksTable[iCount].nStackSize, aGlobalTasksTable[iCount].pArgument, nPriority, &(aGlobalTasksTable[iCount].tTaskHandle)) != pdPASS)
				{
					SeErrorPrint("Freertos create task fail, priority:%d", nPriority);
					break;
				}

				aGlobalTasksTable[iCount].tTaskStatus = SeTaskStatusTypeRunning;
				SeDebugPrint("Add a new Task to schedule list, priority:[%d]", nPriority);
#endif
			}
		}
	}
}

static SeInt8 SeTaskGetNewTaskIndex(void)
{
	SeInt8 iCount;
	for(iCount = 0; iCount < CONFIG_MAX_TASKS; iCount++)
	{
		if(aGlobalTasksTable[iCount].tTaskStatus == SeTaskStatusTypeInvalid)
		{
			return iCount;
		}
	}
	return TASK_IS_INFINITE;
}

SeInt8 SeTaskAdd(SeInt8* iIndex, void* fpTaskRoutine, void* pArgument, SeTaskPriority tTaskPriority, SeUInt32 nTaskStackSize)
{
	SeInt8 iTaskIndex;
	if(iIndex == SeNull || fpTaskRoutine == SeNull)
	{
		SeErrorPrint("Task description parameter is not correct!");
		return SE_RETURN_ERROR;
	}

	iTaskIndex = SeTaskGetNewTaskIndex();
	if(iTaskIndex == TASK_IS_INFINITE)
	{
		SeErrorPrint("Task is no is no longer to create!");
		return SE_RETURN_ERROR;
	}

	aGlobalTasksTable[iTaskIndex].fpRoutine = fpTaskRoutine;
	aGlobalTasksTable[iTaskIndex].pArgument = pArgument;
	aGlobalTasksTable[iTaskIndex].tPriority = tTaskPriority;
	aGlobalTasksTable[iTaskIndex].nStackSize = nTaskStackSize;

	*iIndex = iTaskIndex;

	aGlobalTasksTable[iTaskIndex].tTaskStatus = SeTaskStatusTypeAdded;

	SeDebugPrint("New task added, index is %d.", *iIndex);

	return SE_RETURN_OK;
}

SeInt8 SeTaskStart(SeInt8* iIndex, void* fpTaskRoutine, void* pArgument, SeTaskPriority tTaskPriority, SeUInt32 nTaskStackSize)
{
	if(SeTaskAdd(iIndex, fpTaskRoutine, pArgument, tTaskPriority, nTaskStackSize) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	SeSemaphorePost(tTaskManagerSemaphore);

	return SE_RETURN_OK;
}

SeInt8 SeTaskRemove(SeInt8 iIndex)
{
	if(iIndex < 0 || iIndex >= CONFIG_MAX_TASKS)
	{
		return SE_RETURN_ERROR;
	}

#ifdef CONFIG_USING_FREERTOS
	vTaskDelete(aGlobalTasksTable[iIndex].tTaskHandle);
#endif

	aGlobalTasksTable[iIndex].tTaskStatus = SeTaskStatusTypeInvalid;

	SeDebugPrint("Task deleted, index is %d.", iIndex);

	return SE_RETURN_OK;
}

SeInt8 SeTaskSelfRemove(void)
{
#ifdef CONFIG_USING_FREERTOS
	vTaskDelete(SeNull);
#endif
}

SeInt8 SeTaskSchedule(void)
{
	SeSemaphoreCreate(&tTaskManagerSemaphore);

#ifdef CONFIG_USING_FREERTOS
	xTaskHandle tManagerTaskHandle;

	if(xTaskCreate(SeTaskManagerRoutine, "manager", SE_BUFFER_SIZE_256, SeNull, tskIDLE_PRIORITY + 1, &tManagerTaskHandle) != pdPASS)
	{
		SeErrorPrint("Create manager task fail!");
		return SE_RETURN_ERROR;
	}

	SeDebugPrint("Manager task is creating...");
#endif

	vTaskStartScheduler();

	return SE_RETURN_OK;
}
