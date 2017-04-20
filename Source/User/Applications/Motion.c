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
#include "Odometry.h"
#include "Registers.h"

typedef struct {
	SeUInt32 nEncode;
	SeUInt8 nPwmVal;
	SeUserRollStates tState;
}SeUserRollInfo;

static SeUserRollInfo tLeftRollInfo, tRightRollInfo;

static SeQueue* pTargetPoseQueue = SeNull;

SeUserOdometry tNextOdom;
SeUserOdometry tCurTarget;
SeUserOdometry tCurOdom;

static SeFloat fBaseTicksPerMeter = 0;
static SeFloat fBaseWheelTrack = 0;

static SeFloat SeUserMotionCalcDist(SeUserOdometry tCurrent, SeUserOdometry tTarget)
{
	SeFloat fDy = tTarget.fY - tCurrent.fY;
	SeFloat fDx = tTarget.fX - tCurrent.fX;
	SeFloat fDist = sqrt(fDy*fDy + fDx*fDx);

	return fDist;
}

static SeFloat SeUserMotionCalcTheta(SeUserOdometry tCurrent, SeUserOdometry tTarget)
{
	return (tTarget.fTheta - tCurrent.fTheta);
}

static void SeUserMotionProc(void)
{
	SeUInt32 nLeftCount = tLeftRollInfo.nEncode;
	SeUInt32 nRightCount = tRightRollInfo.nEncode;

	SeBool blArrived = SeFalse;

	tLeftRollInfo.nEncode = 0;
	tRightRollInfo.nEncode = 0;

	if(nLeftCount != 0 && nRightCount != 0)
	{
		//SeDebugPrint("Left=%d,Right=%d", nLeftCount, nRightCount);
		tCurOdom = SeUserOdometryCalculate(nLeftCount, nRightCount);
	}

	/*
	 * synchronize to register
	 */
	SeSpiSlaveSetRegister(BASE_REG_ODOM_X, &tCurOdom.fX, sizeof(SeFloat));
	SeSpiSlaveSetRegister(BASE_REG_ODOM_Y, &tCurOdom.fY, sizeof(SeFloat));
	SeSpiSlaveSetRegister(BASE_REG_ODOM_THETA, &tCurOdom.fTheta, sizeof(SeFloat));

	/*
	 * judge the target has arrived.
	 */
	if(!blArrived)
	{
		/*
		tRightRollInfo.tState = SE_USER_ROLL_FORWARD;
		tLeftRollInfo.tState = SE_USER_ROLL_FORWARD;
		SeUserMotorSet(tLeftRollInfo.tState, 50, tRightRollInfo.tState, 50);
		*/
	}
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
	SeDebugPrint("Set kp : %d.", iKp);
}

void SeUserMotionSetPidKi(SeInt32 iKi)
{
	SeDebugPrint("Set ki : %d.", iKi);
}

void SeUserMotionSetPidKd(SeInt32 iKd)
{
	SeDebugPrint("Set kd : %d.", iKd);
}

void SeUserMotionSetTicksPerMeter(SeFloat fTicksPerMeter)
{
	SeDebugPrint("Set ticks per meter : %f.", fTicksPerMeter);
	fBaseTicksPerMeter = fTicksPerMeter;
}

void SeUserMotionSetWheelTrack(SeFloat fWheelTrack)
{
	SeDebugPrint("Set wheel track : %f.", fWheelTrack);
	fBaseWheelTrack = fWheelTrack;
}

void SeUserMotionSetTargetX(SeFloat fTargetX)
{
	SeDebugPrint("Set target x : %f.", fTargetX);
	tNextOdom.fX = fTargetX;
}

void SeUserMotionSetTargetY(SeFloat fTargetY)
{
	SeDebugPrint("Set target y : %f.", fTargetY);
	tNextOdom.fY = fTargetY;
}

void SeUserMotionSetTargetTheta(SeFloat fTargetTheta)
{
	SeDebugPrint("Set target theta : %f.", fTargetTheta);
	tNextOdom.fTheta = fTargetTheta;
}

void SeUserMotionSetMotor(SeUserRollStates tLeftDir, SeInt8 nLeftPwmDuty, SeUserRollStates tRightDir, SeInt8 nRightPwmDuty)
{
	tLeftRollInfo.tState = tLeftDir;
	tRightRollInfo.tState = tRightDir;

	if(tRightRollInfo.tState == SE_USER_ROLL_FORWARD)
	{
		SeDebugPrint("Right roll set forward!");
	}else if(tRightRollInfo.tState == SE_USER_ROLL_BACKWARD)
	{
		SeDebugPrint("Right roll set backward!");
	}

	if(tLeftRollInfo.tState == SE_USER_ROLL_FORWARD)
	{
		SeDebugPrint("Left roll set forward!");
	}else if(tLeftRollInfo.tState == SE_USER_ROLL_BACKWARD)
	{
		SeDebugPrint("Left roll set backward!");
	}

	SeUserMotorSet(tLeftDir, nLeftPwmDuty, tRightDir, nRightPwmDuty);
}

static SeTaskReturnType SeUserMotionTask(void* pArgument)
{
	SeUserOdometryDescription tOdomDesc;
	SeTimer tMotionTimer;

	while(fBaseTicksPerMeter == 0 || fBaseWheelTrack == 0)
	{
		SeDelayMs(200);
	}

	tOdomDesc.fTicksPerMeter = fBaseTicksPerMeter;
	tOdomDesc.fWheelTrack = fBaseWheelTrack;
	if(SeUserOdometryInit(tOdomDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("Motion odometry init fail!");
		return;
	}
/*
	tMotionTimer.fpSeTimerPreInit = SeStm32f107Timer5Init;
	tMotionTimer.fpSeTimerStart = SeStm32f107Timer5Start;
	tMotionTimer.fpSeTimerStop = SeStm32f107Timer5Stop;
	tMotionTimer.tInterval.iUtcSeconds = 0;
	tMotionTimer.tInterval.iMicroSeconds = CONTROL_DURATION_BY_MS*1000;
	tMotionTimer.tCallback.fpSeTimerCallback = SeUserMotionProc;
	if(SeTimerInit(SE_MOTION_TIMER_INDEX, tMotionTimer) != SE_RETURN_OK)
	{
		SeErrorPrint("Motion timer init fail!");
		return;
	}
	SeTimerStart(SE_MOTION_TIMER_INDEX);
	SeDebugPrint("Motion timer init ok!");
*/
	while(SeTrue)
	{
		SeUserMotionProc();
		//SeDebugPrint("x = %f, y = %f, theta = %f", tCurOdom.fX, tCurOdom.fY, tCurOdom.fTheta);
		SeDelayMs(100);
	}
}

SeInt8 SeUserMotionInit(void)
{
	SeInt8 iMotionTaskIndex;
	SeUserEncoderOperation tEncoderOper;

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

	if(SeTaskAdd(&iMotionTaskIndex, SeUserMotionTask, SeNull, SeTaskPriorityNormal, SE_BUFFER_SIZE_1024) != SE_RETURN_OK)
	{
		SeErrorPrint("Create motion task failure!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("Motion application init ok!");

	return SE_RETURN_OK;
}
