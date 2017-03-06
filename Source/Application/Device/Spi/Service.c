/*
 * Register.c
 *
 *  Created on: 2017年2月23日
 *      Author: seeing
 */

#include "Service.h"

#define SE_DEVICE_SPI_REQUEST_SYNC 0xA5
#define SE_DEVICE_SPI_RESPONSE_SYNC 0xB5

#define SE_DEVICE_SPI_REQUEST_TYPE 0x01
#define SE_DEVICE_SPI_RESPONSE_TYPE 0x00

#define SE_DEVICE_SPI_WRITE 0x01
#define SE_DEVICE_SPI_READ 0x00

#define SE_DEVICE_SPI_RESULT_SUCCESS 0x01
#define SE_DEVICE_SPI_RESULT_FAILURE 0x00

typedef struct{
	SeUInt8 bSyncronize;
	SeUInt16 iSequence;
	SeUInt8 iType;
	SeUInt8 bReadOrWrite;
	SeUInt16 hAddress;
	SeUInt16 nLength;
}__attribute__((packed)) SeDeviceSpiCommReqHead;

typedef struct {
	SeUInt8 bSyncronize;
	SeUInt16 iSequence;
	SeUInt8 iType;
	SeUInt8 iResult;
	SeUInt32 tTimestamp;
	SeUInt16 tTimestampMilliSecond;
	SeUInt16 nLength;
}__attribute__((packed)) SeDeviceSpiCommRepHead;

static SeSemaphoreType tSpiSlaveProcessSemaphore;
static SeUInt8* pSpiSlaveServiceSpace = SeNull;
static SeUInt8 *pTxBuffer = SeNull, *pRxBuffer = SeNull;

typedef struct {
	SeUInt16 hAddress;
	SeSpiSlaveServiceHandle cbServiceHandle;
	SeBool blRegisted;
}SeDeviceSpiHandleInfo;

static SeDeviceSpiHandleInfo pSpiServiceHandles[CONFIG_MAX_SPI_SLAVE_REGISTERS] = {0};

void SeSpiSlaveGetRegister(SeUInt16 hAddress, SeUInt8* pData, SeUInt16 nSize)
{
	memcpy(pData, pSpiSlaveServiceSpace + hAddress, nSize);
}

void SeSpiSlaveSetRegister(SeUInt16 hAddress, SeUInt8* pData, SeUInt16 nSize)
{
	memcpy(pSpiSlaveServiceSpace + hAddress, pData, nSize);
}

static void SeSpiSlaveReceiveRequest(SeUInt8* pRequestData, SeInt16 nRequestLength)
{
	/*
	 * because of hal layer using DMA, the buffer point is pointed to
	 * pTxBuffer and pRxBuffer , so use them directly.
	 */
}

static void SeSpiSlaveMakeResponse(SeUInt8* pResponseData, SeInt16* nResponseLength)
{
	SeDeviceSpiCommReqHead tReqHead;
	SeDeviceSpiCommRepHead tRepHead;
	SeUInt16 iRepSeekPos = 0, iReqStartPos = 0;
	SeUInt16 iCursor;
	SeUInt8 bChecksum = 0;

	for(iReqStartPos = 0; iReqStartPos < CONFIG_DEVICE_SPI_RX_BUFFER_SIZE; iReqStartPos++)
	{
		if(pRxBuffer[iReqStartPos] == SE_DEVICE_SPI_REQUEST_SYNC)
			break;
	}

	memcpy(&tReqHead, pRxBuffer + iReqStartPos, sizeof(SeDeviceSpiCommReqHead));

	tRepHead.bSyncronize = SE_DEVICE_SPI_RESPONSE_SYNC;
	tRepHead.iSequence = tReqHead.iSequence;
	tRepHead.iType = SE_DEVICE_SPI_RESPONSE_TYPE;
	tRepHead.iResult = SE_DEVICE_SPI_RESULT_SUCCESS;
	tRepHead.tTimestamp = 0;
	tRepHead.tTimestampMilliSecond = 0;

	if(tReqHead.bReadOrWrite == SE_DEVICE_SPI_WRITE)
	{
		tRepHead.nLength = 0;
	}else{
		tRepHead.nLength = tReqHead.nLength;
	}

	memcpy(pTxBuffer, &tRepHead, sizeof(SeDeviceSpiCommRepHead));
	iRepSeekPos += sizeof(SeDeviceSpiCommRepHead);

	if(tReqHead.bReadOrWrite == SE_DEVICE_SPI_READ)
	{
		SeSpiSlaveGetRegister(tReqHead.hAddress, (pTxBuffer + iRepSeekPos), tReqHead.nLength);
		iRepSeekPos += tReqHead.nLength;
	}

	for(iCursor = 0; iCursor < iRepSeekPos; iCursor++)
	{
		bChecksum ^= pTxBuffer[iCursor];
	}

	pTxBuffer[iRepSeekPos] = bChecksum;
}

static void SeSpiSlaveLaterProcess(void)
{
	SeSemaphorePost(tSpiSlaveProcessSemaphore);
}

static void SeSpiSlaveProcess(void)
{
	SeDeviceSpiCommReqHead tReqHead;
	SeUInt16 hRegAddr;
	SeUInt16 nReqLen;
	SeUInt8* pReqData;
	SeUInt16 iReqStartPos = 0;
	SeInt16 iCursor;

	for(iReqStartPos = 0; iReqStartPos < CONFIG_DEVICE_SPI_RX_BUFFER_SIZE; iReqStartPos++)
	{
		if(pRxBuffer[iReqStartPos] == SE_DEVICE_SPI_REQUEST_SYNC)
			break;
	}

	if(iReqStartPos >= CONFIG_DEVICE_SPI_RX_BUFFER_SIZE - 1)
	{
		return;
	}

	memcpy(&tReqHead, pRxBuffer + iReqStartPos, sizeof(SeDeviceSpiCommReqHead));

	if(tReqHead.bReadOrWrite == SE_DEVICE_SPI_READ)
		return;

	hRegAddr = tReqHead.hAddress;
	nReqLen = tReqHead.nLength;
	pReqData = &pRxBuffer[iReqStartPos + sizeof(SeDeviceSpiCommReqHead)];
	SeSpiSlaveSetRegister(hRegAddr, pReqData, nReqLen);

	/*
	 * throw register event handle
	 */
	for(iCursor = 0; iCursor < CONFIG_MAX_SPI_SLAVE_REGISTERS; iCursor++)
	{
		if(pSpiServiceHandles[iCursor].hAddress == hRegAddr && pSpiServiceHandles[iCursor].blRegisted == SeTrue)
		{
			pSpiServiceHandles[iCursor].cbServiceHandle(hRegAddr, pReqData, nReqLen);
		}
	}
}

static SeTaskReturnType SeSpiSlaveProcessTask(void* pArgument)
{
	while(SeTrue)
	{
		SeSemaphoreWait(tSpiSlaveProcessSemaphore, SE_SEMAPHORE_INFINITE_WAIT);
		SeSpiSlaveProcess();
	}
}

SeInt8 SeSpiSlaveServiceInit(SeInt8 iSpiSlaveIndex, SeSpiSlaveDescription tSpiSlave, SeInt16 nServiceSpaceSize)
{
	SeInt8 iSpiSlaveTaskIndex;

	pSpiSlaveServiceSpace = SeMemoryAlloc(nServiceSpaceSize);
	if(pSpiSlaveServiceSpace == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(tSpiSlave.tBufferDescription.blAllocated == SeFalse)
	{
		pTxBuffer = SeMemoryAlloc(CONFIG_DEVICE_SPI_TX_BUFFER_SIZE);
		pRxBuffer = SeMemoryAlloc(CONFIG_DEVICE_SPI_RX_BUFFER_SIZE);
		if(pTxBuffer == SeNull || pRxBuffer == SeNull)
		{
			return SE_RETURN_ERROR;
		}
	}

	tSpiSlave.tBufferDescription.pReceiveBuffer = pRxBuffer;
	tSpiSlave.tBufferDescription.pTransmitBuffer = pTxBuffer;
	tSpiSlave.tBufferDescription.nReceiveBufferLength = CONFIG_DEVICE_SPI_RX_BUFFER_SIZE;
	tSpiSlave.tBufferDescription.nTransmitBufferLength = CONFIG_DEVICE_SPI_TX_BUFFER_SIZE;
	tSpiSlave.tBufferDescription.blAllocated = SeTrue;

	tSpiSlave.tCallbacks.fpSeSpiSlaveReceiveRequest = SeSpiSlaveReceiveRequest;
	tSpiSlave.tCallbacks.fpSeSpiSlaveMakeResponse = SeSpiSlaveMakeResponse;
	tSpiSlave.tCallbacks.fpSeSpiSlaveLaterProcess = SeSpiSlaveLaterProcess;

	if(SeSpiSlaveInit(iSpiSlaveIndex, &tSpiSlave) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	SeSemaphoreCreate(&tSpiSlaveProcessSemaphore);

	if(SeTaskAdd(&iSpiSlaveTaskIndex, SeSpiSlaveProcessTask, SeNull, SeTaskPriorityNormal, 1024) != SE_RETURN_OK)
	{
		SeErrorPrint("Create spi slave task failure!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("Create spi slave task success!");


	return SE_RETURN_OK;
}

SeInt8 SeSpiSlaveServiceRegisterHandle(SeUInt16 hAddress, SeSpiSlaveServiceHandle cbServiceHandle)
{
	SeInt16 iCursor;
	SeBool blRegisted = SeFalse;

	for(iCursor = 0; iCursor < CONFIG_MAX_SPI_SLAVE_REGISTERS; iCursor++)
	{
		if(pSpiServiceHandles[iCursor].hAddress == hAddress && pSpiServiceHandles[iCursor].blRegisted == SeTrue)
		{
			pSpiServiceHandles[iCursor].cbServiceHandle = cbServiceHandle;
			return SE_RETURN_OK;
		}
	}

	for(iCursor = 0; iCursor < CONFIG_MAX_SPI_SLAVE_REGISTERS; iCursor++)
	{
		if(pSpiServiceHandles[iCursor].blRegisted == SeFalse)
		{
			pSpiServiceHandles[iCursor].cbServiceHandle = cbServiceHandle;
			pSpiServiceHandles[iCursor].blRegisted = SeTrue;
			blRegisted = SeTrue;
			break;
		}
	}

	if(!blRegisted)
		return SE_RETURN_ERROR;

	return SE_RETURN_OK;
}
