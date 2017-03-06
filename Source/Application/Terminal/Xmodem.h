/*
 * Xmodem.h
 *
 *  Created on: 2016年2月18日
 *      Author: seeing
 */

#ifndef _XMODEM_H_
#define _XMODEM_H_

#include"../../Global/Include.h"

typedef struct
{
	SeUInt8 iXmodemUartIndex;
}SeXmodemDescription;

SeInt8 SeXmodemConfig(SeXmodemDescription tXmodemDescription);

typedef SeInt8 (*SeXmodemWriteData)(SeUInt32 nAddress, SeUInt8* pData, SeUInt16 nLength);

SeInt8 SeXmodemReceive(SeUInt32 nBytesToReceive, SeUInt32* nBytesReceived,
		SeUInt32 nAddress, SeXmodemWriteData fpSeXmodemWriteData);

#endif /* _XMODEM_H_ */
