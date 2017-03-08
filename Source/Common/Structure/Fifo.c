/*
 * Fifo.c
 *
 *  Created on: 2015-12-16
 *      Author: Administrator
 */

#include "Fifo.h"

SeFifo* SeFifoCreate(SeUInt16 nFifoSize)
{
	SeUInt8* pBuf = SeNull;
	SeFifo* pFifo = SeNull;

	if(nFifoSize > CONFIG_MAX_FIFO_SIZE)
		return SeNull;

	pBuf = SeMemoryAlloc(nFifoSize);
	if(pBuf == SeNull)
		return SeNull;

	pFifo = SeMemoryAlloc(sizeof(SeFifo));
	if(pFifo == SeNull)
	{
		SeMemoryFree(pBuf);
		return SeNull;
	}

	pFifo->pBuffer = pBuf;
	pFifo->nSize = 0;
	pFifo->nCapacity = nFifoSize;
	pFifo->iFront = 0;
	pFifo->iRear = 0;

	return pFifo;
}

void SeFifoFinish(SeFifo* pFifo)
{
	if(pFifo == SeNull)
		return;
	if(pFifo->pBuffer != SeNull)
		SeMemoryFree(pFifo->pBuffer);
	SeMemoryFree(pFifo);

	return;
}

SeBool SeFifoJudgeEmpty(SeFifo* pFifo)
{
	if(pFifo == SeNull)
		return SeFalse;
	if(/*(pFifo->iFront == pFifo->iRear) && */(pFifo->nSize == 0))
		return SeTrue;
	return SeFalse;
}

SeBool SeFifoJudgeFull(SeFifo* pFifo)
{
	if(pFifo == SeNull)
		return SeTrue;
	if(/*(pFifo->iFront == pFifo->iRear) && */(pFifo->nSize == pFifo->nCapacity))
		return SeTrue;
	return SeFalse;
}

void SeFifoPush(SeFifo* pFifo, SeUInt8 bData)
{
	*(pFifo->pBuffer + pFifo->iRear) = bData;
	if((pFifo->iRear + 1) == pFifo->nCapacity)
	{
		pFifo->iRear = 0;
	}else{
		pFifo->iRear++;
	}
	pFifo->nSize++;

	return;
}

SeUInt8 SeFifoPop(SeFifo* pFifo)
{
	SeUInt8 bData;
	bData = *(pFifo->pBuffer + pFifo->iFront);
	if((pFifo->iFront + 1) == pFifo->nCapacity)
	{
		pFifo->iFront = 0;
	}else{
		pFifo->iFront++;
	}
	pFifo->nSize--;

	return bData;
}
