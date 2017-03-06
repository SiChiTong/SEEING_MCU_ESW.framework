/*
 * Terminal.c
 *
 *  Created on: 2015-11-28
 *      Author: Administrator
 */


#include "Terminal.h"

static SeChar aInputBuffer[CONFIG_TERMINAL_INPUT_BUFFER_SIZE] = {0};
static SeChar aOutputBuffer[CONFIG_TERMINAL_OUTPUT_BUFFER_SIZE] = {0};
static SeUInt32 nInputCount = 0;
static SeTermCursorInf tCurrentPromptCursor;
static SeBool blEnterKeyPressed = SeTrue;

SeInt8 SeTerminalProcess(void)
{
	SeChar cInput;

	//cInput = getchar();
	cInput = SeGetCh();

	if(cInput == 0x1B)
	{
		/*
		 * key escape
		 */
	}else if(cInput == 0x0D)
	{
		/*
		 * key enter,excute command and recode it
		 */
		blEnterKeyPressed = SeTrue;
	}else if(cInput == 0x08)
	{
		/*
		 * key backspace,delete previous character
		 */
		if(nInputCount > 0)
		{
			aInputBuffer[nInputCount - 1] = '\0';
			nInputCount--;
		}
	}else if(cInput >= 0x20 && cInput <= 0x7E)
	{
		/*
		 * characters
		 */
		if(nInputCount < CONFIG_TERMINAL_INPUT_BUFFER_SIZE)
		{
			aInputBuffer[nInputCount] = cInput;
			nInputCount++;
		}
	}else{

		return SE_RETURN_ERROR;
	}

	if(blEnterKeyPressed)
	{
		SePrintf("\r\n");

		if(nInputCount > 0)
		{
			SeExecuteCommandAndWaitResult(aInputBuffer, aOutputBuffer, CONFIG_TERMINAL_COMMAND_EXECUTE_TIMEOUT);
			SePrintf("\r\n%s\r\n", aOutputBuffer);
		}

		memset(aInputBuffer, 0, sizeof(aInputBuffer));
		nInputCount = 0;

		/*
		 * a new prompt
		 */
		SePrintf("%s", CONFIG_COMMAND_LINE_PROMPT);
		tCurrentPromptCursor = SeGetCurrentTermCursor();

		blEnterKeyPressed = SeFalse;
	}else{
		SeSetCurrentTermCursor(tCurrentPromptCursor);
		SeTermCursorEraseForward();
		SeTermCursorEraseDown();
		SePrintf("%s", aInputBuffer);
	}

	return SE_RETURN_OK;
}

void SeTerminalWelcome(void)
{
	SeWelcomePrint();
}

static SeTaskReturnType SeTerminalProcessTask(void* pArgument)
{
	SeTerminalWelcome();
	while(SeTrue)
	{
		SeTerminalProcess();
		SeDelayMs(100);
	}
}

SeInt8 SeTerminalInit(void)
{
	SeInt8 iTermTaskIndex;

	if(SeTaskAdd(&iTermTaskIndex, SeTerminalProcessTask, SeNull, SeTaskPriorityNormal, 1024) != SE_RETURN_OK)
	{
		SeErrorPrint("Create terminal task failure!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("Create terminal task success!");

	return SE_RETURN_OK;
}
