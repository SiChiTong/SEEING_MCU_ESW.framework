/*
 * Odometry.c
 *
 *  Created on: 2017年3月3日
 *      Author: seeing
 */

#include "Odometry.h"
#include <math.h>

static SeUserOdometryDescription tGlobalOdomDesc;
static SeUserOdometry tCurrentOdometry;

SeInt8 SeUserOdometryInit(SeUserOdometryDescription tOdometryDescription)
{
	if(tOdometryDescription.fWheelTrack == 0)
	{
		return SE_RETURN_ERROR;
	}

	tGlobalOdomDesc = tOdometryDescription;

	tCurrentOdometry.fX = 0;
	tCurrentOdometry.fY = 0;
	tCurrentOdometry.fTheta = 0;

	return SE_RETURN_OK;
}

SeUserOdometry SeUserOdometryCalculate(SeUInt32 nLeftEncoderDelta, SeUInt32 nRightEncoderDelta)
{
	SeUserOdometry tOdom;
	SeFloat fDeltaRight, fDeltaLeft;
	SeFloat fDistAve;
	SeFloat fDeltaTheta;
	SeFloat fDeltaX, fDeltaY;
	SeFloat fDX, fDY;

	fDeltaLeft = nLeftEncoderDelta/tGlobalOdomDesc.fTicksPerMeter;
	fDeltaRight = nRightEncoderDelta/tGlobalOdomDesc.fTicksPerMeter;

	fDistAve = (fDeltaLeft + fDeltaRight)/2.0f;
	fDeltaTheta = (fDeltaRight - fDeltaLeft)/tGlobalOdomDesc.fWheelTrack;

	fDeltaX = cos(fDeltaTheta)*fDistAve;
	fDeltaY = -sin(fDeltaTheta)*fDistAve;

	fDX = cos(tCurrentOdometry.fTheta)*fDeltaX - sin(tCurrentOdometry.fTheta)*fDeltaY;
	fDY = sin(tCurrentOdometry.fTheta)*fDeltaX + cos(tCurrentOdometry.fTheta)*fDeltaY;

	tCurrentOdometry.fX += fDX;
	tCurrentOdometry.fY += fDY;
	tCurrentOdometry.fTheta += fDeltaTheta;

	return tCurrentOdometry;
}

SeUserOdometry SeUserOdometryGetLast(void)
{
	return tCurrentOdometry;
}
