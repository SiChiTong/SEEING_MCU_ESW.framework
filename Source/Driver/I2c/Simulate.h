/*
 * Simulate.h
 *
 *  Created on: 2015-12-2
 *      Author: Administrator
 */

#ifndef _SIMULATE_I2C_H_
#define _SIMULATE_I2C_H_

#include "../../Global/Include.h"

SeInt8 SeSimulateI2cInit(void* pKindOperation);
SeInt8 SeSimulateI2cStart(void* pKindOperation);
SeInt8 SeSimulateI2cStop(void* pKindOperation);
SeInt8 SeSimulateI2cAck(void* pKindOperation);
SeInt8 SeSimulateI2cNack(void* pKindOperation);
SeInt8 SeSimulateI2cRead(void* pKindOperation, SeUInt8* bData, SeBool blAck);
SeInt8 SeSimulateI2cWrite(void* pKindOperation, SeUInt8 bData, SeBool* blAck);

#endif /* _SIMULATE_I2C_H_ */
