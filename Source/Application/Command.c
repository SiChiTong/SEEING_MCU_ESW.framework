/*
 * Command.c
 *
 *  Created on: 2015-11-23
 *      Author: Administrator
 */

#include "Command.h"

#include <string.h>

static SeCommandInf aCmdInf[CONFIG_TERMINAL_MAX_COMMANDS];
static SeUInt16 nCmdCount;

static SeChar aResultBuffer[SE_BUFFER_SIZE_512] = {0};

static SeSemaphoreType tCommandSemaphore;
static SeQueue* pCommandQueue = SeNull;

typedef struct
{
	SeString sCommand;
	SeSemaphoreType tSyncSemaphore;
	SeTimeValue tRequestTime;
	SeTimeValue tResponseTime;
	SeString sResult;
	SeInt8 nResult;
	SeBool blExecuted;
}SeCommandDescription;

static SeInt8 SeCommandParseKeyword(SeString sCommand, SeString sKeyword)
{
	SeString sTemp = SeNull;
	SeChar sCmd[SE_BUFFER_SIZE_256] = {0};

	if(strlen(sCommand) > SE_BUFFER_SIZE_256)
	{
		SeErrorPrint("Input command is too long!");
		return SE_RETURN_ERROR;
	}

	strncpy(sCmd, sCommand, strlen(sCommand));

	sTemp = strtok(sCmd, "(");

	if(sTemp == SeNull)
	{
		strncpy(sKeyword, sCommand, strlen(sCommand));
		return SE_RETURN_OK;
	}

	strncpy(sKeyword, sTemp, strlen(sTemp));

	return SE_RETURN_OK;
}

static SeInt8 SeCommandParseParameter(SeString sCommand, SeLinkList** pParametersList)
{
	SeString sTemp = SeNull;
	SeString sParam = SeNull;
	SeString sEnd = SeNull;
	SeChar sCmd[SE_BUFFER_SIZE_256] = {0};

	if(strlen(sCommand) > SE_BUFFER_SIZE_256)
	{
		SeErrorPrint("Input command is too long!");
		return SE_RETURN_ERROR;
	}

	strncpy(sCmd, sCommand, strlen(sCommand));

	*pParametersList = SeNull;

	sEnd = strchr(sCmd, ')');

	if(sEnd != SeNull)
	{
		*sEnd = '\0';
	}

	sTemp = strchr(sCmd, '(');
	if(sTemp == SeNull)
	{
		/*
		 * there's no parameter in this command
		 */
		return SE_RETURN_OK;
	}

	/*
	 * if the character '(' in command string but ')' is not,
	 * return error.
	 */
	if(sEnd == SeNull)
	{
		SeErrorPrint("If the character '(' in command string but ')' is not.");
		return SE_RETURN_ERROR;
	}

	sTemp++;
	sParam = strtok(sTemp, ",");

	*pParametersList = SeLinkListCreate();

	while(sParam != SeNull)
	{
		/*
		 * new parameter
		 */
		SeCommandParameterInf* pNewParam = SeMemoryAlloc(sizeof(SeCommandParameterInf));
		if(pNewParam == SeNull)
		{
			/*
			 * allocate space failure
			 */
			SeErrorPrint("Allocate memory fail!");
			return SE_RETURN_ERROR;
		}
		pNewParam->nParameterLength = strlen(sParam);
		if(pNewParam->nParameterLength > CONFIG_TERMINAL_PARAMETER_LENGTH)
		{
			/*
			 * the parameter string is too long!
			 */
			SeErrorPrint("The parameter string is too long!");
			return SE_RETURN_ERROR;
		}
		strncpy(pNewParam->sParameter, sParam, pNewParam->nParameterLength);

		SeLinkListAddNode(*pParametersList, pNewParam);

		sParam = strtok(SeNull, ",");
	}

	return SE_RETURN_OK;
}

SeInt8 SeRegisterCommand(SeConstString csCommand, SeCommandProcess cbProcessFunction)
{
	if(cbProcessFunction == SeNull)
	{
		SeErrorPrint("The process function is null!");
		return SE_RETURN_ERROR;
	}

	if(nCmdCount >= CONFIG_TERMINAL_MAX_COMMANDS)
	{
		SeErrorPrint("Can't register new command any more!");
		return SE_RETURN_ERROR;
	}

	strcpy(aCmdInf[nCmdCount].sCommand, csCommand);
	aCmdInf[nCmdCount].nCommandLength = strlen(csCommand);
	aCmdInf[nCmdCount].cbCommandProcess = cbProcessFunction;

	nCmdCount++;

	return SE_RETURN_OK;
}


SeInt8 SeExecuteFullCommand(SeString sFullCommand, SeString sResult)
{
	SeChar sKeyword[CONFIG_TERMINAL_COMMAND_LENGTH] = {0};
	SeLinkList* pParametersList = SeNull;
	SeInt16 nIndex = 0;
	SeInt8 nResult = SE_RETURN_OK;

	if(sResult == SeNull)
	{
		SeErrorPrint("Result string space was not given.");
		return SE_RETURN_ERROR;
	}

	/*
	 * get command key word
	 */
	if(SeCommandParseKeyword(sFullCommand, sKeyword) == SE_RETURN_ERROR)
	{
		SeErrorPrint("Parse keyword fail!");
		return SE_RETURN_ERROR;
	}


	/*
	 * get command parameters list
	 */
	if(SeCommandParseParameter(sFullCommand, &pParametersList) == SE_RETURN_ERROR)
	{
		SeErrorPrint("Parse parameters fail!");
		return SE_RETURN_ERROR;
	}

	/*
	 * judge and execute command
	 */
	for(nIndex = 0; nIndex < nCmdCount; nIndex++)
	{
		if(strncmp(aCmdInf[nIndex].sCommand, sKeyword, aCmdInf[nIndex].nCommandLength) == 0)
		{
			nResult = (aCmdInf[nIndex].cbCommandProcess)(pParametersList, sResult);
			break;
		}
	}

	if(nIndex == nCmdCount)
	{
		SeErrorPrint("Invalid command!");
		strcat(sResult, "ERROR_INVALID_CMD");
		nResult = SE_RETURN_ERROR;
	}

	/*
	 * must free parameters
	 */
	SeLinkList* pNextNode = pParametersList;
	while(!SeLinkListNoNext(pNextNode))
	{
		SeMemoryFree(SeLinkListGetCurrentItem(pNextNode));

		pNextNode = SeLinkListGetNextNode(pNextNode);
	}
	SeLinkListFree(pParametersList);

	return nResult;
}

static SeTaskReturnType SeCommandProcessTask(void* pArgument)
{
	while(SeTrue)
	{
		SeSemaphoreWait(tCommandSemaphore, SE_SEMAPHORE_INFINITE_WAIT);
		if(SeQueueJudgeEmpty(pCommandQueue) != SeTrue)
		{
			/*
			 * Execute a command
			 */
			SeCommandDescription* pCmdItem = SeQueuePop(pCommandQueue);
			if(pCmdItem == SeNull)
			{
				SeErrorPrint("Command queue take a mistake!");
				continue;
			}

			pCmdItem->nResult = SeExecuteFullCommand(pCmdItem->sCommand, pCmdItem->sResult);

			pCmdItem->tResponseTime = SeGetCurrentTimeValue();

			pCmdItem->blExecuted = SeTrue;

			SeSemaphorePost(pCmdItem->tSyncSemaphore);
		}
	}
}

SeInt8 SeExecuteCommandAndWaitResult(SeString sFullCommand, SeString sResult, SeUInt16 nTimeout)
{
	/*
	 * check parameters
	 */
	if(sResult == SeNull)
	{
		SeErrorPrint("Parameter {result} is not correct!");
		return SE_RETURN_ERROR;
	}

	SeCommandDescription* pNewCmdDesc = SeMemoryAlloc(sizeof(SeCommandDescription));
	if(pNewCmdDesc == SeNull)
	{
		SeErrorPrint("Create new command description fail!");
		return SE_RETURN_ERROR;
	}

	if(SeQueueJudgeFull(pCommandQueue) == SeTrue)
	{
		SeErrorPrint("Command queue is full!");
		return SE_RETURN_ERROR;
	}

	memset(aResultBuffer, 0, sizeof(aResultBuffer));

	pNewCmdDesc->sCommand = sFullCommand;
	pNewCmdDesc->sResult = aResultBuffer;
	pNewCmdDesc->tRequestTime = SeGetCurrentTimeValue();
	SeSemaphoreCreate(&(pNewCmdDesc->tSyncSemaphore));
	pNewCmdDesc->blExecuted = SeFalse;

	SeQueuePush(pCommandQueue, pNewCmdDesc);

	SeSemaphorePost(tCommandSemaphore);
	/*
	 * wait for command execute over
	 */
	SeInt8 nRetry = 3;
	SeBool blTimeout = SeTrue;
	while(nRetry--)
	{
		SeSemaphoreWait(pNewCmdDesc->tSyncSemaphore, nTimeout);
		if(pNewCmdDesc->blExecuted == SeTrue)
		{
			blTimeout = SeFalse;
			break;
		}
	}

	/*
	 * make result string
	 */
	SeChar sDoneOrError[16] = {0};
	if(pNewCmdDesc->nResult == SE_RETURN_OK)
	{
		strcat(sDoneOrError, "DONE");
	}else{
		strcat(sDoneOrError, "ERROR");
	}
	sprintf(sResult, "ACK(%s;%s;%ld,%ld,%ld,%ld,%ld)\r\n", pNewCmdDesc->sResult, sDoneOrError,
			pNewCmdDesc->tRequestTime.iUtcSeconds, pNewCmdDesc->tRequestTime.iMicroSeconds, pNewCmdDesc->tResponseTime.iUtcSeconds, pNewCmdDesc->tResponseTime.iMicroSeconds,
			((pNewCmdDesc->tResponseTime.iUtcSeconds - pNewCmdDesc->tRequestTime.iUtcSeconds)*1000 + (pNewCmdDesc->tResponseTime.iMicroSeconds - pNewCmdDesc->tRequestTime.iMicroSeconds)));

	/*
	 * release memory
	 */
	SeSemaphoreDestroy(pNewCmdDesc->tSyncSemaphore);
	SeMemoryFree(pNewCmdDesc);

	if(blTimeout == SeTrue)
	{
		SeErrorPrint("Command execute timeout!");
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeCommandsInit(void)
{
	memset(aCmdInf, 0, (sizeof(SeCommandInf) * CONFIG_TERMINAL_MAX_COMMANDS));
	nCmdCount = 0;

	pCommandQueue = SeQueueCreate(CONFIG_MAX_COMMAND_QUEUE_SIZE);
	if(pCommandQueue == SeNull)
	{
		SeErrorPrint("Create command queue fail!");
		return SE_RETURN_ERROR;
	}

	SeSemaphoreCreate(&tCommandSemaphore);

	SeInt8 iCmdTaskIndex;
	if(SeTaskAdd(&iCmdTaskIndex, SeCommandProcessTask, SeNull, SeTaskPriorityNormal, 1024) != SE_RETURN_OK)
	{
		SeErrorPrint("Create command process task fail!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("Create command process task success!");

	return SE_RETURN_OK;
}

SeInt8 SeCommandsFinish(void)
{

	return SE_RETURN_OK;
}

SeBool SeCommondCheckBuffer(SeUInt8* pInputBuffer, SeInt16 nInputLength)
{
	//if(pInputBuffer[nInputLength - 2] == 0x0D && pInputBuffer[nInputLength - 1] == 0x0A)
	if((pInputBuffer[nInputLength - 2] == 0x0D && pInputBuffer[nInputLength - 1] == 0x0A) || pInputBuffer[nInputLength - 1] == ')')
	{
		return SeTrue;
	}
	return SeFalse;
}
