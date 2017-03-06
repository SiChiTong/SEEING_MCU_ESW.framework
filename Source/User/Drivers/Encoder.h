/*
 * Encoder.h
 *
 *  Created on: 2017年3月1日
 *      Author: seeing
 */

#ifndef _USER_DRIVERS_ENCODER_H_
#define _USER_DRIVERS_ENCODER_H_

#include "../../Global/Include.h"

typedef struct {
	void (*fpSeUserRightEncoderTrigger)(void);
	void (*fpSeUserLeftEncoderTrigger)(void);
	void (*fpSeUserEncoderSteady)(void);
}SeUserEncoderOperation;

SeInt8 SeUserEncoderInit(SeUserEncoderOperation tUserEncoderOperation);

#endif /* SOURCE_USER_DRIVERS_ENCODER_H_ */
