/*
 * Odometry.h
 *
 *  Created on: 2017年3月3日
 *      Author: seeing
 */

#ifndef _USER_APPLICATIONS_ODOMETRY_H_
#define _USER_APPLICATIONS_ODOMETRY_H_

#include "../../Global/Include.h"

typedef struct {
	SeFloat fTicksPerMeter;
	SeFloat fWheelTrack;
}SeUserOdometryDescription;

typedef struct {
	SeFloat fX;
	SeFloat fY;
	SeFloat fTheta;
}SeUserOdometry;

SeInt8 SeUserOdometryInit(SeUserOdometryDescription tOdometryDescription);

SeUserOdometry SeUserOdometryCalculate(SeUInt32 nLeftEncoderCount, SeUInt32 nRightEncoderCount);
SeUserOdometry SeUserOdometryGetLast(void);

#endif /* SOURCE_USER_APPLICATIONS_ODOMETRY_H_ */
