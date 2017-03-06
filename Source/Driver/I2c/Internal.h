/*
 * Internal.h
 *
 *  Created on: 2015-12-2
 *      Author: Administrator
 */

#ifndef _INTERNAL_I2C_H_
#define _INTERNAL_I2C_H_

#include "../../Global/Include.h"

SeInt8 SeInternalI2cInit(void* pKindOperation);
SeInt8 SeInternalI2cStart(void* pKindOperation);
SeInt8 SeInternalI2cStop(void* pKindOperation);
SeInt8 SeInternalI2cAck(void* pKindOperation);
SeInt8 SeInternalI2cNack(void* pKindOperation);
SeInt8 SeInternalI2cRead(void* pKindOperation, SeUInt8* bData, SeBool blAck);
SeInt8 SeInternalI2cWrite(void* pKindOperation, SeUInt8 bData, SeBool* blAck);

#endif /* _INTERNAL_I2C_H_ */
