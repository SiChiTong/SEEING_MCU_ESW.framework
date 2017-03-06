/*
 * Message.c
 *
 *  Created on: 2016年4月23日
 *      Author: seeing
 */

#include "Message.h"

static SeTaskReturnType SeTerminalMessageProcessTask(void* pArgument)
{
	SeTerminalMessageDescription* pDescription = (SeTerminalMessageDescription*)pArgument;
	SeInt16 iBufferCursor = 0;
	while(SeTrue)
	{
		SeUInt8 bData;
		SeInt16 nRead;
		SeInt16 nWrite;
		if((SeUartRead(pDescription->iUartIndex, &bData, 1, &nRead) == SE_RETURN_OK) && nRead == 1)
		{
			if(iBufferCursor >= (pDescription->nReceiveBufferSize - 1))
			{
				memset(pDescription->pReceiveBuffer, 0, pDescription->nReceiveBufferSize);
				iBufferCursor = 0;
			}else{
				pDescription->pReceiveBuffer[iBufferCursor] = bData;
				iBufferCursor++;
				if(bData == 0x0D)
				{
					memset(pDescription->pTransmitBuffer, 0, pDescription->nTransmitBufferSize);
					SeExecuteCommandAndWaitResult(pDescription->pReceiveBuffer, pDescription->pTransmitBuffer, CONFIG_TERMINAL_COMMAND_EXECUTE_TIMEOUT);
					SeUartWrite(pDescription->iUartIndex, pDescription->pTransmitBuffer, strlen(pDescription->pTransmitBuffer), &nWrite);
					memset(pDescription->pReceiveBuffer, 0, pDescription->nReceiveBufferSize);
					iBufferCursor = 0;
				}
			}
			continue;
		}
		SeDelayMs(10);
	}

	return;
}

SeInt8 SeTerminalMessageInit(SeTerminalMessageDescription* pTerminalMessageDescription)
{
	if(pTerminalMessageDescription == SeNull)
	{
		SeErrorPrint("Terminal message description is not correct!");
		return SE_RETURN_ERROR;
	}

	if(pTerminalMessageDescription->pReceiveBuffer == SeNull)
	{
		pTerminalMessageDescription->pReceiveBuffer = SeMemoryAlloc(CONFIG_TERMINAL_INPUT_BUFFER_SIZE);
		if(pTerminalMessageDescription->pReceiveBuffer == SeNull)
		{
			SeErrorPrint("Create terminal message buffer fail!");
			return SE_RETURN_ERROR;
		}
	}
	pTerminalMessageDescription->nReceiveBufferSize = CONFIG_TERMINAL_INPUT_BUFFER_SIZE;

	if(pTerminalMessageDescription->pTransmitBuffer == SeNull)
	{
		pTerminalMessageDescription->pTransmitBuffer = SeMemoryAlloc(CONFIG_TERMINAL_OUTPUT_BUFFER_SIZE);
		if(pTerminalMessageDescription->pTransmitBuffer == SeNull)
		{
			SeErrorPrint("Create terminal message buffer fail!");
			SeMemoryFree(pTerminalMessageDescription->pReceiveBuffer);
			return SE_RETURN_ERROR;
		}
	}
	pTerminalMessageDescription->nTransmitBufferSize = CONFIG_TERMINAL_INPUT_BUFFER_SIZE;

	SeInt8 iMessageTaskIndex;
	if(SeTaskAdd(&iMessageTaskIndex, SeTerminalMessageProcessTask, pTerminalMessageDescription, SeTaskPriorityNormal, 512) != SE_RETURN_OK)
	{
		SeErrorPrint("Create terminal message process task fail!");
		SeMemoryFree(pTerminalMessageDescription->pReceiveBuffer);
		SeMemoryFree(pTerminalMessageDescription->pTransmitBuffer);
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("Create terminal message process task success!");
	return SE_RETURN_OK;
}
