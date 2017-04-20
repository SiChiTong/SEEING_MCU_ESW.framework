/*
 * String.c
 *
 *  Created on: 2015-11-19
 *      Author: Administrator
 */
#include "String.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>

static SeStdioOperation* pSeStdio = SeNull;

int __io_putchar(int ch)
{
	return SePutCh((SeChar)ch);
}

int __io_getchar(void)
{
	return SeGetCh();
}

int _read (int file, char *ptr, int len)
{
	int DataIdx;
	int Datalen = 0;
	len = 1;

    switch(file)
    {
		case STDIN_FILENO:
			for (DataIdx = 0; DataIdx < len; DataIdx++)
			{
				int cInput = __io_getchar();
				if(cInput == 0)
					continue;

				*ptr++ = cInput;
				Datalen++;
			}

			return Datalen;
		default:
			return -1;
    }
}

int _write(int file, char *ptr, int len)
{
	int DataIdx;

    switch (file)
    {
		case STDOUT_FILENO: /*stdout*/
			for (DataIdx = 0; DataIdx < len; DataIdx++)
			{
			   __io_putchar( *ptr++ );
			}
			return len;
		default:
			return -1;
    }
    return len;
}

caddr_t _sbrk(int incr)
{
	extern char end asm("end");
	register char *pStack asm("sp");

	static char *s_pHeapEnd;

	if (!s_pHeapEnd)
		s_pHeapEnd = &end;

	if (s_pHeapEnd + incr > pStack)
		return (caddr_t)-1;

	char *pOldHeapEnd = s_pHeapEnd;
	s_pHeapEnd += incr;
	return (caddr_t)pOldHeapEnd;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	switch (file)
	{
		case STDOUT_FILENO:
		case STDERR_FILENO:
		case STDIN_FILENO:
			return 1;
		default:
			return 0;
	}
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _open(char *path, int flags, ...)
{
	return -1;
}

int _close(int file)
{
	return -1;
}

SeInt8 SeStdConfig(SeStdioOperation* pStdio)
{
	if(pStdio == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pSeStdio = pStdio;

	if(pSeStdio->fpSeStdioInit() == SE_RETURN_ERROR)
	{

		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeChar SePutCh(SeChar cOut)
{
	if(pSeStdio->fpSeFputc != SeNull)
	{
		pSeStdio->fpSeFputc(cOut);
	}
	return (cOut);
}

SeChar SeGetCh(void)
{
	SeUInt8 bGetChar;

	if(pSeStdio->fpSeFgetc != SeNull)
	{
		if(pSeStdio->fpSeFgetc(&bGetChar) != SE_RETURN_OK)
		{
			return 0;
		}
	}

	return bGetChar;
}

/*
void SePrintf(SeConstString csFormat, ...)
{
	va_list vArgs;
	va_start(vArgs, csFormat);
	printf(csFormat, vArgs);
	va_end(vArgs);

#ifdef CONFIG_USING_FFLUSH
	fflush(stdout);
#endif
}
*/

SeInt8 SeScanf(SeConstString csFormat, ...)
{
	va_list vArgs;
	SeInt32 nResult;
	SeInt32 nGot = 0;
	SeInt16 nIndex = 0;
	SeChar sScanString[SE_BUFFER_SIZE_128] = {0};

	for(nGot = 0; nGot < SE_BUFFER_SIZE_128; nGot++)
	{
		SeChar cInput = 0;
		SeInt32 nRetry = STDIN_TIMEOUT;
		while((cInput = SeGetCh()) == 0)
		{
			nRetry--;
			if(nRetry == 0)
				break;
		}

		if(nRetry == 0)
		{
			if(nGot == 0)
			{
				return SE_RETURN_TIMEOUT;
			}else{
				break;
			}
		}
		sScanString[nIndex] = cInput;
		nIndex++;
	}

	va_start(vArgs, csFormat);
	nResult = vsscanf(sScanString, csFormat, vArgs);
	va_end(vArgs);

	return SE_RETURN_OK;
}


void SeNormalPrint(SeConstString csFormat, ...)
{
	va_list vArgs;
	SeInt32 nResult;

	SeChar sPrintString[SE_BUFFER_SIZE_256] = {0};

	va_start(vArgs, csFormat);
	nResult = vsprintf(sPrintString, csFormat, vArgs);
	va_end(vArgs);

	if(nResult >= 0)
	{
		printf(sPrintString);
		printf("\r\n");
	}
}

void SeErrorPrint(SeConstString csFormat, ...)
{
	va_list vArgs;
	SeInt32 nResult;

	SeChar sPrintString[SE_BUFFER_SIZE_256] = {0};

	va_start(vArgs, csFormat);
	nResult = vsprintf(sPrintString, csFormat, vArgs);
	va_end(vArgs);

	if(nResult >= 0)
	{
		SeTermPrintWithColor("Error", SeTermForeColorWhite, SeTermBackColorRed);
		printf(":[");
		printf(sPrintString);
		printf("]\r\n");
	}
}

void SeDebugPrint(SeConstString csFormat, ...)
{
	va_list vArgs;
	SeInt32 nResult;

	SeChar sPrintString[SE_BUFFER_SIZE_256] = {0};

	va_start(vArgs, csFormat);
	nResult = vsprintf(sPrintString, csFormat, vArgs);
	va_end(vArgs);

	if(nResult >= 0)
	{
		SeTermPrintWithColor("Debug", SeTermBackColorBlack, SeTermForeColorYellow);
		printf(":[");
		printf(sPrintString);
		printf("]\r\n");
	}
}

void SeDataPrint(SeConstString csTitle, SeUInt8* pData, SeUInt16 nLength)
{
	SeUInt16 iCursor;
	printf("%s, Data Position:0x%0x, Length:%d\r\n", csTitle, pData, nLength);
	for(iCursor = 0; iCursor < nLength; iCursor++)
	{
		printf(" 0x%02X ", *(pData + iCursor));
		if(iCursor != 0 && iCursor%8 == 0)
		{
			printf("\r\n");
			continue;
		}
		printf(",");
	}
	printf("\r\n");
}

static SeTermCursorInf tGlobalTermCursorInfo;
static SeUInt8 iPreviousPercentage = 0;

void SeProgressStart(SeConstString csTitle)
{
	SeTermCursorInvisible();
	printf("\r\n%s\r\n", csTitle);
	tGlobalTermCursorInfo = SeGetCurrentTermCursor();
}

void SeProgressPrint(SeUInt8 iPercentage, SeUInt8 nStep)
{
	SeInt8 iBlockCharIndex = 0;

	if(iPercentage > iPreviousPercentage)
	{
		SeSetCurrentTermCursor(tGlobalTermCursorInfo);
		for(iBlockCharIndex = 0; iBlockCharIndex < iPercentage/nStep; iBlockCharIndex++)
		{
			SeTermPrintWithReverse(" ");
		}
		printf("    [%d%%%]", iPercentage);
	}

	iPreviousPercentage = iPercentage;
}

void SeProgressFinish(void)
{
	SeTermCursorVisible();
	printf("\r\nDone!\r\n");
}

static iRunningFlag = 0;

void SeHoldLinePrintStart(void)
{
	SeTermCursorInvisible();
	tGlobalTermCursorInfo = SeGetCurrentTermCursor();
}

void SeHoldLinePrint(SeConstString csFormat, ...)
{
	va_list vArgs;
	SeInt32 nResult;

	SeChar sPrintString[SE_BUFFER_SIZE_256] = {0};

	va_start(vArgs, csFormat);
	nResult = vsprintf(sPrintString, csFormat, vArgs);
	va_end(vArgs);

	iRunningFlag++;
	if(iRunningFlag == 1)
	{
		strcat(sPrintString, "    -");
	}else if(iRunningFlag == 2)
	{
		strcat(sPrintString, "    \\");
	}else if(iRunningFlag == 3)
	{
		strcat(sPrintString, "    |");
	}else if(iRunningFlag == 4)
	{
		strcat(sPrintString, "    /");
		iRunningFlag = 0;
	}

	if(nResult >= 0)
	{
		SeSetCurrentTermCursor(tGlobalTermCursorInfo);
		printf(sPrintString);
	}
}

void SeHoldLinePrintFinish(void)
{
	SeTermCursorVisible();
	printf("\r\nDone!\r\n");
}

void SeWelcomePrint(void)
{
	SeUInt8 iCount;
	SeUInt16 nLineWidth;

	SeChar sPrintString[SE_BUFFER_SIZE_256] = {0};

	printf("\r\n");

	nLineWidth = strlen(CONFIG_PROJECT_DESCRIPTION) + 8;

	for(iCount = 0; iCount < (nLineWidth/2 + 1); iCount++)
	{
		strcat(sPrintString, "* ");
	}
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	/*
	 * print project name
	 */
	strcat(sPrintString, "*");
	for(iCount = strlen(sPrintString); iCount < nLineWidth - 1; iCount++)
	{
		strcat(sPrintString, " ");
	}
	strcat(sPrintString, "*");
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	strcat(sPrintString, "*");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, "Project Name:    ");
	strcat(sPrintString, CONFIG_PROJECT_NAME);
	for(iCount = strlen(sPrintString); iCount < nLineWidth - 1; iCount++)
	{
		strcat(sPrintString, " ");
	}
	strcat(sPrintString, "*");
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	/*
	 * print project firmware version
	 */
	strcat(sPrintString, "*");
	for(iCount = strlen(sPrintString); iCount < nLineWidth - 1; iCount++)
	{
		strcat(sPrintString, " ");
	}
	strcat(sPrintString, "*");
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	strcat(sPrintString, "*");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, "Firmware Version:    ");
	strcat(sPrintString, CONFIG_PROJECT_VERSION);
	for(iCount = strlen(sPrintString); iCount < nLineWidth - 1; iCount++)
	{
		strcat(sPrintString, " ");
	}
	strcat(sPrintString, "*");
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	/*
	 * print project platform
	 */
	strcat(sPrintString, "*");
	for(iCount = strlen(sPrintString); iCount < nLineWidth - 1; iCount++)
	{
		strcat(sPrintString, " ");
	}
	strcat(sPrintString, "*");
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	strcat(sPrintString, "*");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, "Project Platform:    ");
	strcat(sPrintString, CONFIG_PROJECT_PLATFORM);
	for(iCount = strlen(sPrintString); iCount < nLineWidth - 1; iCount++)
	{
		strcat(sPrintString, " ");
	}
	strcat(sPrintString, "*");
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	/*
	 * print project description
	 */
	strcat(sPrintString, "*");
	for(iCount = strlen(sPrintString); iCount < nLineWidth - 1; iCount++)
	{
		strcat(sPrintString, " ");
	}
	strcat(sPrintString, "*");
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	strcat(sPrintString, "*");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, " ");
	strcat(sPrintString, CONFIG_PROJECT_DESCRIPTION);
	for(iCount = strlen(sPrintString); iCount < nLineWidth - 1; iCount++)
	{
		strcat(sPrintString, " ");
	}
	strcat(sPrintString, "*");
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	/*
	 * last line
	 */
	strcat(sPrintString, "*");
	for(iCount = strlen(sPrintString); iCount < nLineWidth - 1; iCount++)
	{
		strcat(sPrintString, " ");
	}
	strcat(sPrintString, "*");
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	for(iCount = 0; iCount < (nLineWidth/2 + 1); iCount++)
	{
		strcat(sPrintString, "* ");
	}
	printf("%s\r\n", sPrintString);
	memset(sPrintString, 0, sizeof(sPrintString));

	printf("\r\n");
}
