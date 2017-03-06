/*
 * Pid.c
 *
 *  Created on: 2017年2月25日
 *      Author: seeing
 */

#include "Pid.h"

/*
 * PID algorithm prototype:
 *
 * u(k) = u(k-1)+((Kp+KiT+Kd/T)e(k)-(Kp+2Kd/T)e(k-1)+(Kd/T)e(k-2))
 *
 * For optimum the complexity, make step0, step1, step2 as temporary variable
 *
 * a0 = Kp+KiT+Kd/T
 * a1 = Kp+2Kd/T
 * a2 = Kd/T
 *
 * so, the algorithm prototype change into:
 *
 * u(k) = u(k-1)+(a0*e(k)-a1*e(k-1)+a2*e(k-2))
 *
 */
typedef struct {
	SeFloat fReference;
	SeFloat fError;
	SeFloat fLastError;
	SeFloat fPreviousError;
	SeFloat fSumError;
	SeFloat fLastOutput;
}SePidInfo;

typedef struct {
	SePidOption tOption;
	SePidInfo tInfo;
	SeFloat fStep0;
	SeFloat fStep1;
	SeFloat fStep2;
}SePidType;

static SePidType aPid[MAX_PID] = {0};

SeInt8 SePidInit(SeInt8 iIndex, SePidOption tPidOption)
{
	if(iIndex < 0 || iIndex >= MAX_PID)
	{
		return SE_RETURN_ERROR;
	}

	aPid[iIndex].tOption = tPidOption;

	aPid[iIndex].fStep0 = tPidOption.fKp + tPidOption.fKi * tPidOption.fPeriod + tPidOption.fKd / tPidOption.fPeriod;
	aPid[iIndex].fStep1 = tPidOption.fKp + 2 * tPidOption.fKd / tPidOption.fPeriod;
	aPid[iIndex].fStep2 = tPidOption.fKd / tPidOption.fPeriod;

	return SE_RETURN_OK;
}

void SePidResetValue(SeInt8 iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_PID)
	{
		return;
	}

	memset(&aPid[iIndex], 0, sizeof(SePidInfo));
}

static SeFloat SePidIncrementalCalc(SePidType* pPid, SeFloat fCurrentValue, SeFloat fFeedback, SeFloat fNextValue)
{
	SeFloat fOutput;

	pPid->tInfo.fError = fCurrentValue - fFeedback;

	fOutput = pPid->tInfo.fLastOutput + pPid->fStep0*pPid->tInfo.fError - pPid->fStep1*pPid->tInfo.fLastError + pPid->fStep2*pPid->tInfo.fPreviousError;

	if(fOutput > pPid->tOption.fMaxValue)
		fOutput = pPid->tOption.fMaxValue;
	if(fOutput < pPid->tOption.fMinValue)
		fOutput = pPid->tOption.fMinValue;

	pPid->tInfo.fLastOutput = fOutput;
	pPid->tInfo.fPreviousError = pPid->tInfo.fLastError;
	pPid->tInfo.fLastError = pPid->tInfo.fError;

	return fOutput;
}

static SeFloat SePidPositionalCalc(SePidType* pPid, SeFloat fCurrentValue, SeFloat fFeedback, SeFloat fNextValue)
{
	SeFloat fOutput;

	pPid->tInfo.fError = fCurrentValue - fFeedback;
	pPid->tInfo.fSumError += pPid->tInfo.fError;

	fOutput = pPid->tOption.fKp*pPid->tInfo.fError + pPid->tOption.fKi*pPid->tInfo.fSumError + pPid->tOption.fKd*(pPid->tInfo.fError - pPid->tInfo.fLastError);

	if(fOutput > pPid->tOption.fMaxValue)
		fOutput = pPid->tOption.fMaxValue;
	if(fOutput < pPid->tOption.fMinValue)
		fOutput = pPid->tOption.fMinValue;

	pPid->tInfo.fLastOutput = fOutput;
	pPid->tInfo.fPreviousError = pPid->tInfo.fLastError;
	pPid->tInfo.fLastError = pPid->tInfo.fError;

	return fOutput;
}

SeFloat SePidCalculate(SeInt8 iIndex, SeFloat fCurrentValue, SeFloat fFeedback, SeFloat fNextValue)
{
	if(iIndex < 0 || iIndex >= MAX_PID)
	{
		return 0;
	}

	switch(aPid[iIndex].tOption.tPidMethod)
	{
		case SE_PID_TYPE_POSITIONAL:
			return SePidPositionalCalc(&aPid[iIndex], fCurrentValue, fFeedback, fNextValue);
		case SE_PID_TYPE_INCREMENTAL:
			return SePidIncrementalCalc(&aPid[iIndex], fCurrentValue, fFeedback, fNextValue);
		default:return 0;
	}
}
