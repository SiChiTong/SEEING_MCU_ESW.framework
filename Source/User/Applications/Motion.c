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
static SeFloat fBaseTicksPerMeter = 0;
static SeFloat fBaseWheelTrack = 0;

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
	SeDebugPrint("Set kp :%d.", iKp);
}
void SeUserMotionSetPidKi(SeInt32 iKi)
{
	SeDebugPrint("Set ki :%d.", iKi);
}
void SeUserMotionSetPidKd(SeInt32 iKd)
{
	SeDebugPrint("Set kd :%d.", iKd);
}
void SeUserMotionSetTicksPerMeter(SeFloat fTicksPerMeter)
{
	SeDebugPrint("Set ticks per meter :%d.", (SeInt32)fTicksPerMeter);
	fBaseTicksPerMeter = fTicksPerMeter;
}
void SeUserMotionSetWheelTrack(SeFloat fWheelTrack)
{
	SeDebugPrint("Set wheel track :%d.", (SeInt32)fWheelTrack);
	fBaseWheelTrack = fWheelTrack;
}
void SeUserMotionSetControlDuration(SeFloat fDuration)
{
	SeDebugPrint("Set duration :%d ms.", (SeInt32)(fDuration*1000));
	fControlDuration = fDuration;
}

static SeTaskReturnType SeUserMotionTask(void* pArgument)
{
	SeTimer tMotionTimer;
	SeUserOdometryDescription tOdomDesc;

	while(fControlDuration == 0 || fBaseTicksPerMeter == 0 || fBaseWheelTrack == 0)
	{
		SeDelayMs(200);
	}

	tMotionTimer.fpSeTimerPreInit = SeStm32f107Timer5Init;
	tMotionTimer.fpSeTimerStart = SeStm32f107Timer5Start;
	tMotionTimer.fpSeTimerStop = SeStm32f107Timer5Stop;
	tMotionTimer.tInterval.iUtcSeconds = (SeUInt64)(fControlDuration*1000/1000);
	tMotionTimer.tInterval.iMicroSeconds = ((SeUInt64)(fControlDuration*1000)%1000)*1000;
	tMotionTimer.tCallback.fpSeTimerCallback = SeUserMotionProc;
	if(SeTimerInit(SE_MOTION_TIMER_INDEX, tMotionTimer) != SE_RETURN_OK)
	{
		SeErrorPrint("Motion timer init fail!");
		return;
	}

	SeTimerStart(SE_MOTION_TIMER_INDEX);

	SeDebugPrint("Motion process timer init ok!");

	tOdomDesc.fDuration = fControlDuration;
	tOdomDesc.fTicksPerMeter = fBaseTicksPerMeter;
	tOdomDesc.fWheelTrack = fBaseWheelTrack;
	if(SeUserOdometryInit(tOdomDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("Motion odometry init fail!");
		return;
	}

	SeDebugPrint("Motion odometry init ok!");

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
