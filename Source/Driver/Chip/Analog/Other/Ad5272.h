/*
 * Ad5272.h
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#ifndef _AD5272_H_
#define _AD5272_H_

#include "../../../../Global/Include.h"

/*
 * Ad5272 hardware description
 */
typedef struct
{
	SeUInt8 iAd5272I2cIndex;
	SeUInt8 bAd5272DeviceAddress;	
}SeAd5272Description;

/*
 * Ad5272 interface function
 */
SeInt8 SeAd5272Init(SeAd5272Description tAd5272Description);
SeInt8 SeAd5272WriteData(SeUInt8 hAddress, SeUInt16 iData);
SeInt8 SeAd5272ReadData(SeUInt8 hAddress, SeUInt16 *pData);
SeInt8 SeAd5272SetResistance(SeUInt8 hAddress, SeFloat fResistance);
SeInt8 SeAd5272ReadResistance(SeUInt8 hAddress, SeFloat *pResistance);
SeInt8 SeAd5272ReadRdacReg(SeUInt8 hAddress, SeUInt16 *pRdacData);
SeInt8 SeAd5272SetRdacStore(SeUInt8 hAddress);
SeInt8 SeAd5272IsSetRdacStoreOK(SeUInt8 hAddress);

#endif
