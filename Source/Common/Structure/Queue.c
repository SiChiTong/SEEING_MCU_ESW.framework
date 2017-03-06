/*
 * Queue.c
 *
 *  Created on: 2015-12-8
 *      Author: Administrator
 */


#include "Queue.h"

SeQueue* SeQueueCreate(SeUInt16 nCapacity)
{
	SeQueue* pQueue = SeNull;
	pQueue = SeMemoryAlloc(sizeof(SeQueue));
	if(pQueue == SeNull)
	{
		return SeNull;
	}
	pQueue->pItems = SeMemoryAlloc(nCapacity*sizeof(void*));
	if(pQueue->pItems == SeNull)
	{
		SeMemoryFree(pQueue);
		return SeNull;
	}

	pQueue->nCapacity = nCapacity;
	pQueue->iFront = 0;
	pQueue->iRear = 0;
	pQueue->nSize = 0;

	return pQueue;
}

void SeQueueFinish(SeQueue* pQueue)
{
	if(pQueue == SeNull)
	{
		return;
	}
	if(pQueue->pItems == SeNull)
	{
		return;
	}

	memset(pQueue->pItems, 0, ((pQueue->nCapacity)*sizeof(void*)));
	SeMemoryFree(pQueue->pItems);
	SeMemoryFree(pQueue);
	return;
}

SeBool SeQueueJudgeFull(SeQueue* pQueue)
{
	if(pQueue == SeNull)
		return SeTrue;

	if(pQueue->nCapacity == pQueue->nSize)
		return SeTrue;

	return SeFalse;
}

SeBool SeQueueJudgeEmpty(SeQueue* pQueue)
{
	if(pQueue == SeNull)
		return SeFalse;

	if(pQueue->nSize > 0)
		return SeFalse;

	return SeTrue;
}

void SeQueuePush(SeQueue* pQueue, void* pItem)
{
	if(pQueue == SeNull || pItem == SeNull)
		return;

	pQueue->pItems[pQueue->iRear] = pItem;
	if((pQueue->iRear + 1) == pQueue->nCapacity)
	{
		pQueue->iRear = 0;
	}else{
		pQueue->iRear++;
	}
	pQueue->nSize++;

	return;
}

void* SeQueuePop(SeQueue* pQueue)
{
	void* pItem = SeNull;

	if(pQueue == SeNull)
		return SeNull;

	pItem = pQueue->pItems[pQueue->iFront];
	if((pQueue->iFront + 1) == pQueue->nCapacity)
	{
		pQueue->iFront = 0;
	}else{
		pQueue->iFront++;
	}
	pQueue->nSize--;

	return pItem;
}
