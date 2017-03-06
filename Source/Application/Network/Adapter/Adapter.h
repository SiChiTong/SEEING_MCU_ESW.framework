/*
 * Adapter.h
 *
 *  Created on: 2016年1月8日
 *      Author: seeing
 */

#ifndef _NETWORK_ADAPTER_H_
#define _NETWORK_ADAPTER_H_

#include "../../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
#ifdef CONFIG_USING_LWIP
	struct netconn *pConnection;
#endif
#ifdef CONFIG_USING_LINUX
	SeInt16 tSocket;
#endif
	SeNetworkConnectionType tNetworkConnectionType;
	SeBool blBlock;
	SeUInt32 nTimeoutValue;
}SeNetworkConnectionHandle;

SeInt8 SeNetworkAdapterCreate(SeNetworkConnectionHandle* pNewNetworkConnectionHandle, SeNetworkConnectionType tNetworkConnectionType);
SeInt8 SeNetworkAdapterConnect(SeNetworkConnectionHandle tNetworkConnectionHandle, SeIpAddress tRemoteIp, SeIpPort tRemotePort);
SeInt8 SeNetworkAdapterClose(SeNetworkConnectionHandle tNetworkConnectionHandle);
SeInt8 SeNetworkAdapterBind(SeNetworkConnectionHandle tNetworkConnectionHandle, SeIpAddress tLocalIp, SeIpPort tLocalPort);
SeInt8 SeNetworkAdapterListen(SeNetworkConnectionHandle tNetworkConnectionHandle);
SeInt8 SeNetworkAdapterAccept(SeNetworkConnectionHandle* pAcceptedNetworkConnectionHandle, SeNetworkConnectionHandle tListeningNetworkConnectionHandle);

SeInt8 SeNetworkAdapterSetBlock(SeNetworkConnectionHandle* pNewNetworkConnectionHandle, SeBool blBlock, SeUInt32 nTimeoutValue);

SeInt8 SeNetworkAdapterReceive(SeNetworkConnectionHandle tNetworkConnectionHandle, SeUInt8* pData, SeUInt16 nBytesToReceive, SeUInt16* nBytesReceived);
SeInt8 SeNetworkAdapterSend(SeNetworkConnectionHandle tNetworkConnectionHandle, SeUInt8* pData, SeUInt16 nBytesToSend, SeUInt16* nBytesSent);

SeInt8 SeNetworkAdapterReceiveFrom(SeNetworkConnectionHandle tNetworkConnectionHandle, SeUInt8* pData, SeUInt16 nBytesToReceive, SeUInt16* nBytesReceived, SeIpAddress* pRemoteIp, SeIpPort* pRemotePort);
SeInt8 SeNetworkAdapterSendTo(SeNetworkConnectionHandle tNetworkConnectionHandle, SeUInt8* pData, SeUInt16 nBytesToSend, SeUInt16* nBytesSent, SeIpAddress tRemoteIp, SeIpPort tRemotePort);


#ifdef __cplusplus
}
#endif

#endif /* _NETWORK_ADAPTER_H_ */
