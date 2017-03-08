/*
 * Register.h
 *
 *  Created on: 2017年2月25日
 *      Author: seeing
 */

#ifndef _USER_APPLICATIONS_REGISTERS_H_
#define _USER_APPLICATIONS_REGISTERS_H_

#include "../../Global/Include.h"

enum{
	BASE_REG_PID_KP = 0x00,
	BASE_REG_PID_KI = 0x04,
	BASE_REG_PID_KD = 0x08,
	BASE_REG_PID_KO = 0x0C,

	BASE_REG_DIST_PER_PULSE = 0x20,
	BASE_REG_WHEEL_TRACK = 0x24,
	BASE_REG_CNTL_DURATION = 0x28,

	BASE_REG_TARGET_X = 0x30,
	BASE_REG_TARGET_Y = 0x34,
	BASE_REG_TARGET_THETA = 0x38,

	BASE_REG_ODOM_X = 0x40,
	BASE_REG_ODOM_Y = 0x44,
	BASE_REG_ODOM_THETA = 0x48,

	BASE_REG_END = 0x50,
};



void SeUserRegisterPidKpHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize);
void SeUserRegisterPidKiHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize);
void SeUserRegisterPidKdHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize);
void SeUserRegisterTicksPerMeterHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize);
void SeUserRegisterWheelTrackHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize);
void SeUserRegisterControlDurationHandle(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize);


#endif /* SOURCE_USER_APPLICATIONS_REGISTER_H_ */
