/*
 * Gpio.h
 *
 *  Created on: 2015-12-4
 *      Author: Administrator
 */

#ifndef _GPIO_H_
#define _GPIO_H_

#include "../../Global/Include.h"

SeInt8 SeGpioInit(SeInt8 iIndex, SeGpioDescription* pGpio);
SeInt8 SeGpioSetBit(SeInt8 iIndex, SeUInt8 bBit);
SeInt8 SeGpioGetBit(SeInt8 iIndex, SeUInt8* bBit);

#endif /* _GPIO_H_ */
