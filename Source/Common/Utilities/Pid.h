/*
 * Pid.h
 *
 *  Created on: 2017年2月25日
 *      Author: seeing
 */

#ifndef _PID_H_
#define _PID_H_

#include "../../Global/Include.h"

#ifdef CONFIG_MAX_PID_CALCULATOR
#define MAX_PID CONFIG_MAX_PID_CALCULATOR
#else
#define MAX_PID 2
#endif

typedef enum {
	SE_PID_TYPE_POSITIONAL,
	SE_PID_TYPE_INCREMENTAL,
}SePidMethod;

typedef struct {
	SePidMethod tPidMethod;
	SeFloat fKp;
	SeFloat fKi;
	SeFloat fKd;
	SeFloat fPeriod;
	SeFloat fMaxValue;
	SeFloat fMinValue;
}SePidOption;

SeInt8 SePidInit(SeInt8 iIndex, SePidOption tPidOption);
void SePidResetValue(SeInt8 iIndex);
SeFloat SePidCalculate(SeInt8 iIndex, SeFloat fCurrentValue, SeFloat fFeedback, SeFloat fNextValue);

#endif /* SOURCE_COMMON_UTILITIES_PID_H_ */
