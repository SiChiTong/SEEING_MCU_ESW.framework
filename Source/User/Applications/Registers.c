/*
 * Register.c
 *
 *  Created on: 2017年2月25日
 *      Author: seeing
 */

#include "Registers.h"
#include "Motion.h"

void SeUserRegisterPidKpHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize)
{
	SeInt32 iKp;
	if(nSize == sizeof(SeInt32))
		memcpy(&iKp, pData, nSize);
	SeUserMotionSetPidKp(iKp);
}

void SeUserRegisterPidKiHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize)
{
	SeInt32 iKi;
	if(nSize == sizeof(SeInt32))
		memcpy(&iKi, pData, nSize);
	SeUserMotionSetPidKi(iKi);
}

void SeUserRegisterPidKdHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize)
{
	SeInt32 iKd;
	if(nSize == sizeof(SeInt32))
		memcpy(&iKd, pData, nSize);
	SeUserMotionSetPidKd(iKd);
}

void SeUserRegisterTicksPerMeterHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize)
{
	SeFloat fTicksPerMeter;
	if(nSize == sizeof(SeFloat))
		memcpy(&fTicksPerMeter, pData, nSize);
	SeUserMotionSetTicksPerMeter(fTicksPerMeter);
}

void SeUserRegisterWheelTrackHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize)
{
	SeFloat fWheelTrack;
	if(nSize == sizeof(SeFloat))
		memcpy(&fWheelTrack, pData, nSize);
	SeUserMotionSetWheelTrack(fWheelTrack);
}

void SeUserRegisterTargetXHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize)
{
	SeFloat fTargetX;
	if(nSize == sizeof(SeFloat))
		memcpy(&fTargetX, pData, nSize);

}

void SeUserRegisterTargetYHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize)
{
	SeFloat fTargetY;
	if(nSize == sizeof(SeFloat))
		memcpy(&fTargetY, pData, nSize);
}

void SeUserRegisterTargetThetaHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize)
{
	SeFloat fTargetTheta;
	if(nSize == sizeof(SeFloat))
		memcpy(&fTargetTheta, pData, nSize);
}
