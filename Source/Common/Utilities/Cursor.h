/*
 * Cursor.h
 *
 *  Created on: 2015-11-19
 *      Author: Administrator
 */

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	SeInt16 nRow;
	SeInt16 nColumn;
}SeTermCursorInf;

typedef enum{
	SeTermBackColorBlack = 40,
	SeTermBackColorRed,
	SeTermBackColorGreen,
	SeTermBackColorYellow,
	SeTermBackColorBlue,
	SeTermBackColorMagenta,
	SeTermBackColorCyan,
	SeTermBackColorWhite,
}SeTermBackColor;

typedef enum{
	SeTermForeColorBlack = 30,
	SeTermForeColorRed,
	SeTermForeColorGreen,
	SeTermForeColorYellow,
	SeTermForeColorBlue,
	SeTermForeColorMagenta,
	SeTermForeColorCyan,
	SeTermForeColorWhite,
}SeTermForeColor;

void SeTermCursorHome(SeInt16 nRow, SeInt16 nColumn);
void SeTermCursorUp(SeInt16 nCount);
void SeTermCursorDown(SeInt16 nCount);
void SeTermCursorForward(SeInt16 nCount);
void SeTermCursorBackward(SeInt16 nCount);
void SeTermCursorForcePosition(SeInt16 nRow, SeInt16 nColumn);
SeTermCursorInf SeGetCurrentTermCursor(void);
void SeSetCurrentTermCursor(SeTermCursorInf tTermCursorInfo);

void SeTermCursorVisible(void);
void SeTermCursorInvisible(void);

void SeTermCursorEraseForward(void);
void SeTermCursorEraseBackward(void);
void SeTermCursorEraseDown(void);
void SeTermCursorEraseUp(void);

void SeTermPrintWithBlink(SeString sContent);
void SeTermPrintWithReverse(SeString sContent);
void SeTermPrintWithColor(SeString sContent, SeTermBackColor tBackColor, SeTermForeColor tForeColor);

#ifdef __cplusplus
}
#endif

#endif /* _CURSOR_H_ */
