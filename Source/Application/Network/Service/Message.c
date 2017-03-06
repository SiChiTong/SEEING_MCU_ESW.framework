/*
 * Message.c
 *
 *  Created on: 2016年3月5日
 *      Author: seeing
 */

#include "Message.h"

typedef struct
{
	SeNetworkConnectionHandle tConnection;
}SeNetworkMessageClientDescription;

static SeTaskReturnType SeNetworkMessageClientProcessTask(void* pArgument)
{
	SeNetworkMessageClientDescription* pClientDesc = (SeNetworkMessageClientDescription*)pArgument;

	SeUInt8* pReceiveBuffer = SeNull;
	SeUInt8* pTransmitBuffer = SeNull;
	SeInt16 nReceiveBufferSize = 0;
	SeInt16 nTransmitBufferSize = 0;

	SeInt16 iBufferCursor = 0;

	if(pReceiveBuffer == SeNull)
	{
		pReceiveBuffer = SeMemoryAlloc(CONFIG_TERMINAL_INPUT_BUFFER_SIZE);
		if(pReceiveBuffer == SeNull)
		{
			SeErrorPrint("Create network message buffer fail!");
			return;
		}
	}
	nReceiveBufferSize = CONFIG_TERMINAL_INPUT_BUFFER_SIZE;

	if(pTransmitBuffer == SeNull)
	{
		pTransmitBuffer = SeMemoryAlloc(CONFIG_TERMINAL_OUTPUT_BUFFER_SIZE);
		if(pTransmitBuffer == SeNull)
		{
			SeErrorPrint("Create network message buffer fail!");
			SeMemoryFree(pReceiveBuffer);
			return;
		}
	}
	nTransmitBufferSize = CONFIG_TERMINAL_INPUT_BUFFER_SIZE;

	while(SeTrue)
	{
		SeInt16 nRead;
		SeInt16 nWrite;

		SeInt8 nResult = SeNetworkAdapterReceive(pClientDesc->tConnection, pReceiveBuffer, nReceiveBufferSize, &nRead);
		if(nResult == SE_RETURN_OK)
		{
			if(iBufferCursor >= (nReceiveBufferSize - 1))
			{
				memset(pReceiveBuffer, 0, nReceiveBufferSize);
				iBufferCursor = 0;
			}else{
				iBufferCursor += nRead;
				if(SeCommondCheckBuffer(pReceiveBuffer, iBufferCursor) == SeTrue)
				{
					memset(pTransmitBuffer, 0, nTransmitBufferSize);
					SeExecuteCommandAndWaitResult(pReceiveBuffer, pTransmitBuffer, CONFIG_TERMINAL_COMMAND_EXECUTE_TIMEOUT);
					SeNetworkAdapterSend(pClientDesc->tConnection, pTransmitBuffer, strlen(pTransmitBuffer), &nWrite);
					memset(pReceiveBuffer, 0, nReceiveBufferSize);
					iBufferCursor = 0;
				}
			}
			continue;
		}else if(nResult == SE_RETURN_ERROR)
		{
			break;
		}
		SeDelayMs(10);
	}

	SeDebugPrint("Ready to quit network message service!");

	SeNetworkAdapterClose(pClientDesc->tConnection);
	SeMemoryFree(pReceiveBuffer);
	SeMemoryFree(pTransmitBuffer);
	SeMemoryFree(pClientDesc);

	SeTaskSelfRemove();

	return;
}

static SeTaskReturnType SeNetworkMessageProcessTask(void* pArgument)
{
	SeNetworkMessageDescription* pDescription = (SeNetworkMessageDescription*)pArgument;

	SeNetworkConnectionHandle tConnectionHandle;

	/*
	 * create tcp connection
	 */
	if(SeNetworkAdapterCreate(&tConnectionHandle, SeNetworkTcpConnection) != SE_RETURN_OK)
	{
		SeErrorPrint("Create network message connection fail!");
		return;
	}

	/*
	 * bind
	 */
	SeIpAddress tIpAddr = {0, 0, 0, 0};
	if(SeNetworkAdapterBind(tConnectionHandle, tIpAddr, pDescription->tPort) != SE_RETURN_OK)
	{
		SeErrorPrint("Create network message connection fail!");
		return;
	}

	/*
	 * listen
	 */
	SeNetworkAdapterListen(tConnectionHandle);

	SeNetworkAdapterSetBlock(&tConnectionHandle, SeFalse, pDescription->nTimeout);

	while(SeTrue)
	{
		SeNetworkConnectionHandle tAcceptHandle;
		if(SeNetworkAdapterAccept(&tAcceptHandle, tConnectionHandle) == SE_RETURN_OK)
		{
			SeNetworkMessageClientDescription* pNetworkMessageClientDescription = SeMemoryAlloc(sizeof(SeNetworkMessageClientDescription));
			if(pNetworkMessageClientDescription == SeNull)
			{
				SeErrorPrint("No memory to allocate description space!");
				break;
			}

			pNetworkMessageClientDescription->tConnection = tAcceptHandle;

			/*
			 * Create new message service
			 */
			SeInt8 iClientMessageTaskIndex;
			if(SeTaskStart(&iClientMessageTaskIndex, SeNetworkMessageClientProcessTask, pNetworkMessageClientDescription, SeTaskPriorityNormal, 1024) != SE_RETURN_OK)
			{
				SeErrorPrint("Create network client message process task fail!");
				break;
			}

			SeDebugPrint("Create a new network message service success!");
		}
	}

	SeNetworkAdapterClose(tConnectionHandle);

	return;
}

SeInt8 SeNetworkMessageInit(SeNetworkMessageDescription* pNetworkMessageDescription)
{
	if(pNetworkMessageDescription == SeNull)
	{
		SeErrorPrint("Network message description is not correct!");
		return SE_RETURN_ERROR;
	}

	SeInt8 iMessageTaskIndex;
	if(SeTaskAdd(&iMessageTaskIndex, SeNetworkMessageProcessTask, pNetworkMessageDescription, SeTaskPriorityNormal, 1024) != SE_RETURN_OK)
	{
		SeErrorPrint("Create network message process task fail!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("Create network message process task success!");
	return SE_RETURN_OK;
}
