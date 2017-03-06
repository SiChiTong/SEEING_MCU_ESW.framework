/*
 * Message.h
 *
 *  Created on: 2016年3月5日
 *      Author: seeing
 */

#ifndef _NETWORK_SERVICE_MESSAGE_H_
#define _NETWORK_SERVICE_MESSAGE_H_

#include "../../../Global/Include.h"

typedef struct
{
	SeIpPort tPort;
	SeInt16 nTimeout;
}SeNetworkMessageDescription;

SeInt8 SeNetworkMessageInit(SeNetworkMessageDescription* pNetworkMessageDescription);

#endif /* _NETWORK_SERVICE_MESSAGE_H_ */
