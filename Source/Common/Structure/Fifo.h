/*
 * Fifo.h
 *
 *  Created on: 2015-12-16
 *      Author: Administrator
 */

#ifndef _FIFO_H_
#define _FIFO_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	SeUInt8* pBuffer;
	SeUInt16 iFront;
	SeUInt16 iRear;
	SeUInt16 nSize;
	SeUInt16 nCapacity;
}SeFifo;

SeFifo* SeFifoCreate(SeUInt16 nFifoSize);
void SeFifoFinish(SeFifo* pFifo);

SeBool SeFifoJudgeEmpty(SeFifo* pFifo);
SeBool SeFifoJudgeFull(SeFifo* pFifo);

void SeFifoPush(SeFifo* pFifo, SeUInt8 bData);
SeUInt8 SeFifoPop(SeFifo* pFifo);

#ifdef __cplusplus
}
#endif

#endif /* _FIFO_H_ */
