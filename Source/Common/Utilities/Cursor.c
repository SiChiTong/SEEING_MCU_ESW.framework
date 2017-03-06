/*
 * Cli.c
 *
 *  Created on: 2015-11-19
 *      Author: Administrator
 */


#include "Cursor.h"

#include <string.h>


static SeTermCursorInf tCurrentCursor;


void SeTermCursorHome(SeInt16 nRow, SeInt16 nColumn)
{
	SePrintf("\033[%d;%dH", nRow, nColumn);
}

void SeTermCursorUp(SeInt16 nCount)
{
	SePrintf("\033[%dA", nCount);
}

void SeTermCursorDown(SeInt16 nCount)
{
	SePrintf("\033[%dB", nCount);
}

void SeTermCursorForward(SeInt16 nCount)
{
	SePrintf("\033[%dC", nCount);
}

void SeTermCursorBackward(SeInt16 nCount)
{
	SePrintf("\033[%dD", nCount);
}

void SeTermCursorForcePosition(SeInt16 nRow, SeInt16 nColumn)
{
	SePrintf("\033[%d;%df", nRow, nColumn);
}

SeTermCursorInf SeGetCurrentTermCursor(void)
{
	SeTermCursorInf tCursor;
	SePrintf("\033[6n");
	SeScanf("\033[%d;%dR", &tCursor.nRow, &tCursor.nColumn);
	return tCursor;
}

void SeSetCurrentTermCursor(SeTermCursorInf tTermCursorInfo)
{
	SeTermCursorForcePosition(tTermCursorInfo.nRow, tTermCursorInfo.nColumn);
}

void SeTermCursorVisible(void)
{
	SePrintf("\033[?25h");
}

void SeTermCursorInvisible(void)
{
	SePrintf("\033[?25l");
}

void SeTermCursorEraseForward(void)
{
	SePrintf("\033[K");
}

void SeTermCursorEraseBackward(void)
{
	SePrintf("\033[1K");
}

void SeTermCursorEraseDown(void)
{
	SePrintf("\033[J");
}

void SeTermCursorEraseUp(void)
{
	SePrintf("\033[1J");
}

void SeTermPrintWithBlink(SeString sContent)
{
	SePrintf("\033[5m%s\033[0m", sContent);
}

void SeTermPrintWithReverse(SeString sContent)
{
	SePrintf("\033[7m%s\033[0m", sContent);
}

void SeTermPrintWithColor(SeString sContent, SeTermBackColor tBackColor, SeTermForeColor tForeColor)
{
	SePrintf("\033[%d;%dm%s\033[0m", tBackColor, tForeColor, sContent);
}
