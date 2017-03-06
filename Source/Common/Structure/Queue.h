/*
 * Queue.h
 *
 *  Created on: 2015-12-8
 *      Author: Administrator
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	void** pItems;
	SeUInt16 iFront;
	SeUInt16 iRear;
	SeUInt16 nSize;
	SeUInt16 nCapacity;
}SeQueue;

SeQueue* SeQueueCreate(SeUInt16 nCapacity);
void SeQueueFinish(SeQueue* pQueue);

SeBool SeQueueJudgeFull(SeQueue* pQueue);
SeBool SeQueueJudgeEmpty(SeQueue* pQueue);

void SeQueuePush(SeQueue* pQueue, void* pItem);
void* SeQueuePop(SeQueue* pQueue);

#ifdef __cplusplus
}
#endif

#endif /* _QUEUE_H_ */
