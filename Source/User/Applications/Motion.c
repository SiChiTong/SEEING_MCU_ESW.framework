/*
 * Motion.c
 *
 *  Created on: 2017年2月28日
 *      Author: seeing
 */

#include "Motion.h"
#include "../Descriptions.h"
#include "../../Platform/Stm32f107/Stm32f10x.h"
#include "../Drivers/Encoder.h"
#include "../Drivers/Motor.h"
#include "Odometry.h"

typedef struct {
	SeUInt64 nEncode;
	SeUInt8 nPwmVal;
	SeUserRollStates tState;
}SeUserRollInfo;

static SeUserRollInfo tLeftRollInfo, tRightRollInfo;

typedef struct {

}SeUserMotionInfo;

typedef struct {

}SeUserMotionDescription;

static SeFloat fControlDuration = 0;

static void SeUserMotionProc(void)
{

}

static void SeUserRightEncTrig(void)
{
	if(tRightRollInfo.tState == SE_USER_ROLL_FORWARD)
	{
		tRightRollInfo.nEncode++;
	}else if(tRightRollInfo.tState == SE_USER_ROLL_BACKWARD)
	{
		tRightRollInfo.nEncode--;
	}
}

static void SeUserLeftEncTrig(void)
{
	if(tLeftRollInfo.tState == SE_USER_ROLL_FORWARD)
	{
		tLeftRollInfo.nEncode++;
	}else if(tLeftRollInfo.tState == SE_USER_ROLL_BACKWARD)
	{
		tLeftRollInfo.nEncode--;
	}
}

static void SeUserEncSteady(void)
{
	tLeftRollInfo.nEncode = 0;
	tLeftRollInfo.tState = SE_USER_ROLL_STOP;

	tRightRollInfo.nEncode = 0;
	tRightRollInfo.tState = SE_USER_ROLL_STOP;
}

void SeUserMotionSetPidKp(SeInt32 iKp)
{

}
void SeUserMotionSetPidKi(SeInt32 iKi)
{

}
void SeUserMotionSetPidKd(SeInt32 iKd)
{

}
void SeUserMotionSetTicksPerMeter(SeFloat fTicksPerMeter)
{

}
void SeUserMotionSetWheelTrack(SeFloat fWheelTrack)
{

}
void SeUserMotionSetControlDuration(SeFloat fDuration)
{
	fControlDuration = fDuration;
}

static SeTaskReturnType SeUserMotionTask(void* pArgument)
{
	SeTimer tMotionTimer;
	SeUserOdometryDescription tOdomDesc;

	while(fControlDuration == 0)
	{
		SeDelayMs(100);
	}

	tMotionTimer.fpSeTimerPreInit = SeStm32f107Timer5Init;
	tMotionTimer.fpSeTimerStart = SeStm32f107Timer5Start;
	tMotionTimer.fpSeTimerStop = SeStm32f107Timer5Stop;
	tMotionTimer.tInterval.iUtcSeconds = 0;
	tMotionTimer.tInterval.iMicroSeconds = /*TODO*/;
	tMotionTimer.tCallback.fpSeTimerCallback = SeUserMotionProc;
	if(SeTimerInit(SE_MOTION_TIMER_INDEX, tMotionTimer) != SE_RETURN_OK)
	{
		SeErrorPrint("Motion application init ok!");
		return;
	}

	SeTimerStart(SE_MOTION_TIMER_INDEX);

	SeDebugPrint("Motion process timer init ok!");

	while(SeTrue)
	{
		SeDelayMs(100);
	}
}

SeInt8 SeUserMotionInit(void)
{
	SeUserEncoderOperation tEncoderOper;
	SeInt8 iMotionTaskIndex;

	tEncoderOper.fpSeUserLeftEncoderTrigger = SeUserLeftEncTrig;
	tEncoderOper.fpSeUserRightEncoderTrigger = SeUserRightEncTrig;
	tEncoderOper.fpSeUserEncoderSteady = SeUserEncSteady;
	if(SeUserEncoderInit(tEncoderOper) != SE_RETURN_OK)
	{
		SeErrorPrint("Encoder init failure!");
		return SE_RETURN_ERROR;
	}

	if(SeUserMotorInit() != SE_RETURN_OK)
	{
		SeErrorPrint("PWM init failure!");
		return SE_RETURN_ERROR;
	}

	if(SeTaskAdd(&iMotionTaskIndex, SeUserMotionTask, SeNull, SeTaskPriorityNormal, 1024) != SE_RETURN_OK)
	{
		SeErrorPrint("Create motion task failure!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("Motion application init ok!");

	return SE_RETURN_OK;
}
