/*
 * Motion.h
 *
 *  Created on: 2017年2月28日
 *      Author: seeing
 */

#ifndef _USER_APPLICATIONS_MOTION_H_
#define _USER_APPLICATIONS_MOTION_H_

#include "../../Global/Include.h"
#include "../Drivers/Motor.h"

#define CONTROL_DURATION_BY_MS 100

SeInt8 SeUserMotionInit(void);

void SeUserMotionSetPidKp(SeInt32 iKp);
void SeUserMotionSetPidKi(SeInt32 iKi);
void SeUserMotionSetPidKd(SeInt32 iKd);
void SeUserMotionSetTicksPerMeter(SeFloat fTicksPerMeter);
void SeUserMotionSetWheelTrack(SeFloat fWheelTrack);
void SeUserMotionSetControlDuration(SeFloat fDuration);
void SeUserMotionSetTargetX(SeFloat fTargetX);
void SeUserMotionSetTargetY(SeFloat fTargetY);
void SeUserMotionSetTargetTheta(SeFloat fTargetTheta);

void SeUserMotionSetMotor(SeUserRollStates tLeftDir, SeInt8 nLeftPwmDuty, SeUserRollStates tRightDir, SeInt8 nRightPwmDuty);

#endif /* SOURCE_USER_APPLICATIONS_MOTION_H_ */
