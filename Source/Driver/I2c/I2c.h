/*
 * I2c.h
 *
 *  Created on: 2015-12-1
 *      Author: Administrator
 */

#ifndef _I2C_H_
#define _I2C_H_

#include "../../Global/Include.h"

SeInt8 SeI2cInit(SeInt8 iIndex, SeI2cBusDescription* pI2c);
SeInt8 SeI2cStart(SeInt8 iIndex);
SeInt8 SeI2cStop(SeInt8 iIndex);
SeInt8 SeI2cAck(SeInt8 iIndex);
SeInt8 SeI2cNack(SeInt8 iIndex);
SeInt8 SeI2cRead(SeInt8 iIndex, SeUInt8* bData, SeBool blAck);
SeInt8 SeI2cWrite(SeInt8 iIndex, SeUInt8 bData, SeBool* blAck);

#endif /* _I2C_H_ */
