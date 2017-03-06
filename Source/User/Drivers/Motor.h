/*
 * Motor.h
 *
 *  Created on: 2017年3月1日
 *      Author: seeing
 */

#ifndef _USER_DRIVERS_MOTOR_H_
#define _USER_DRIVERS_MOTOR_H_

#include "../../Global/Include.h"

typedef enum {
	SE_USER_ROLL_STOP = 0,
	SE_USER_ROLL_FORWARD,
	SE_USER_ROLL_BACKWARD,
}SeUserRollStates;

SeInt8 SeUserMotorInit(void);

void SeUserMotorSet(SeUserRollStates tLeftDir, SeInt8 nLeftPwmDuty, SeUserRollStates tRightDir, SeInt8 nRightPwmDuty);


#endif /* SOURCE_USER_DRIVERS_MOTOR_H_ */
