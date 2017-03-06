/*
 * Message.h
 *
 *  Created on: 2016年4月23日
 *      Author: seeing
 */

#ifndef _TERMINAL_MESSAGE_H_
#define _TERMINAL_MESSAGE_H_

#include "../../../Global/Include.h"

typedef struct
{
	SeInt8 iUartIndex;
	SeUInt8* pReceiveBuffer;
	SeUInt8* pTransmitBuffer;
	SeInt16 nReceiveBufferSize;
	SeInt16 nTransmitBufferSize;
}SeTerminalMessageDescription;

SeInt8 SeTerminalMessageInit(SeTerminalMessageDescription* pTerminalMessageDescription);

#endif /* _TERMINAL_MESSAGE_H_ */
