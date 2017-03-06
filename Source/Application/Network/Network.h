/*
 * Network.h
 *
 *  Created on: 2016年1月8日
 *      Author: seeing
 */

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

SeInt8 SeNetworkInit(SeNetworkDescription* pNetworkDescription, SeNetworkOperation* pNetworkOperation);

SeInt8 SeNetworkSetIpAddress(SeIpAddress tIp);
SeInt8 SeNetworkGetIpAddress(SeIpAddress* tIp);

#ifdef __cplusplus
}
#endif

#endif /* _NETWORK_H_ */
