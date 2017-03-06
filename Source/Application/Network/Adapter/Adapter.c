/*
 * Tcp.c
 *
 *  Created on: 2016年1月8日
 *      Author: seeing
 */

#include "Adapter.h"

SeInt8 SeNetworkAdapterCreate(SeNetworkConnectionHandle* pNewNetworkConnectionHandle, SeNetworkConnectionType tNetworkConnectionType)
{
	if(pNewNetworkConnectionHandle == SeNull)
	{
		SeErrorPrint("Network connection handle is not correct!");
		return SE_RETURN_ERROR;
	}
#ifdef CONFIG_USING_LWIP
	if(tNetworkConnectionType == SeNetworkTcpConnection)
	{
		pNewNetworkConnectionHandle->pConnection = netconn_new(NETCONN_TCP);
	}else if(tNetworkConnectionType == SeNetworkUdpConnection)
	{
		pNewNetworkConnectionHandle->pConnection = netconn_new(NETCONN_UDP);
	}
	if(pNewNetworkConnectionHandle->pConnection == SeNull)
	{
		SeErrorPrint("Network create new connection fail!");
		return SE_RETURN_ERROR;
	}
#endif

#ifdef CONFIG_USING_LINUX

#endif

	pNewNetworkConnectionHandle->tNetworkConnectionType = tNetworkConnectionType;

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterConnect(SeNetworkConnectionHandle tNetworkConnectionHandle, SeIpAddress tRemoteIp, SeIpPort tRemotePort)
{
#ifdef CONFIG_USING_LWIP
	struct ip_addr tRemoteIpAddr;
	SeUInt16 nRemotePortNo;

	IP4_ADDR(&tRemoteIpAddr, tRemoteIp.aIpAddress[0], tRemoteIp.aIpAddress[1], tRemoteIp.aIpAddress[2], tRemoteIp.aIpAddress[3]);
	//nRemotePortNo = htons(tRemotePort);
	nRemotePortNo = tRemotePort;

	if(netconn_connect(tNetworkConnectionHandle.pConnection, &tRemoteIpAddr, nRemotePortNo) != ERR_OK)
	{
		SeErrorPrint("Network connect fail!");
		return SE_RETURN_ERROR;
	}

#endif

#ifdef CONFIG_USING_LINUX

#endif

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterClose(SeNetworkConnectionHandle tNetworkConnectionHandle)
{
#ifdef CONFIG_USING_LWIP
	if(tNetworkConnectionHandle.tNetworkConnectionType == SeNetworkTcpConnection)
	{
		netconn_close(tNetworkConnectionHandle.pConnection);
		netconn_delete(tNetworkConnectionHandle.pConnection);
	}else if(tNetworkConnectionHandle.tNetworkConnectionType == SeNetworkUdpConnection)
	{
		netconn_disconnect(tNetworkConnectionHandle.pConnection);
		netconn_delete(tNetworkConnectionHandle.pConnection);
	}
#endif

#ifdef CONFIG_USING_LINUX

#endif

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterSetBlock(SeNetworkConnectionHandle* pNewNetworkConnectionHandle, SeBool blBlock, SeUInt32 nTimeoutValue)
{
#ifdef CONFIG_USING_LWIP
	if(blBlock == SeFalse)
	{
		netconn_set_recvtimeout(pNewNetworkConnectionHandle->pConnection, nTimeoutValue);
	}
#endif

	pNewNetworkConnectionHandle->blBlock = blBlock;
	pNewNetworkConnectionHandle->nTimeoutValue = nTimeoutValue;

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterBind(SeNetworkConnectionHandle tNetworkConnectionHandle, SeIpAddress tLocalIp, SeIpPort tLocalPort)
{
#ifdef CONFIG_USING_LWIP
	struct ip_addr tLocalIpAddr;
	SeUInt16 nLocalPortNo;

	IP4_ADDR(&tLocalIpAddr, tLocalIp.aIpAddress[0], tLocalIp.aIpAddress[1], tLocalIp.aIpAddress[2], tLocalIp.aIpAddress[3]);
	//nLocalPortNo = htons(tLocalPort);
	nLocalPortNo = tLocalPort;

	if(netconn_bind(tNetworkConnectionHandle.pConnection, &tLocalIpAddr, nLocalPortNo) != ERR_OK)
	{
		SeErrorPrint("Network bind fail!");
		return SE_RETURN_ERROR;
	}
#endif

#ifdef CONFIG_USING_LINUX

#endif

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterListen(SeNetworkConnectionHandle tNetworkConnectionHandle)
{
#ifdef CONFIG_USING_LWIP
	netconn_listen(tNetworkConnectionHandle.pConnection);
#endif

#ifdef CONFIG_USING_LINUX

#endif

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterAccept(SeNetworkConnectionHandle* pAcceptedNetworkConnectionHandle, SeNetworkConnectionHandle tListeningNetworkConnectionHandle)
{
	if(pAcceptedNetworkConnectionHandle == SeNull)
	{
		SeErrorPrint("Network accept parameter is not correct!");
		return SE_RETURN_ERROR;
	}
#ifdef CONFIG_USING_LWIP
	struct netconn* pNewCon;
	err_t tError;
	tError = netconn_accept(tListeningNetworkConnectionHandle.pConnection, &pNewCon);
	if(tError == ERR_OK)
	{
		pAcceptedNetworkConnectionHandle->tNetworkConnectionType = SeNetworkTcpConnection;
		pAcceptedNetworkConnectionHandle->pConnection = pNewCon;
	}else if(tError == ERR_TIMEOUT)
	{
		//SeErrorPrint("Network accept timeout!");
		return SE_RETURN_TIMEOUT;
	}else
	{
		SeErrorPrint("Network accept fail!");
		return SE_RETURN_ERROR;
	}
	if(pNewCon == SeNull)
	{
		SeErrorPrint("Network accept a new connection fail!");
		return SE_RETURN_ERROR;
	}
#endif

#ifdef CONFIG_USING_LINUX

#endif

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterReceive(SeNetworkConnectionHandle tNetworkConnectionHandle, SeUInt8* pData, SeUInt16 nBytesToReceive, SeUInt16* nBytesReceived)
{
	if(pData == SeNull || nBytesReceived == SeNull)
	{
		SeErrorPrint("Network receive parameter is not correct!");
		return SE_RETURN_ERROR;
	}

	*nBytesReceived = 0;

#ifdef CONFIG_USING_LWIP
	struct netbuf* pNetBuf;
	err_t tError;
	tError = netconn_recv(tNetworkConnectionHandle.pConnection, &pNetBuf);
	if(tError == ERR_OK)
	{
		if(tNetworkConnectionHandle.tNetworkConnectionType == SeNetworkTcpConnection)
		{
			do{
				void* pDataPtr;
				SeUInt16 nDataLen;
				netbuf_data(pNetBuf, &pDataPtr, &nDataLen);
				memcpy((pData + *nBytesReceived), pDataPtr, nDataLen);
				*nBytesReceived += nDataLen;
			}while(netbuf_next(pNetBuf) >= 0);

		}else if(tNetworkConnectionHandle.tNetworkConnectionType == SeNetworkUdpConnection)
		{
			if(nBytesToReceive < pNetBuf->p->len)
			{
				SeErrorPrint("Receive bytes is too few!");
				return SE_RETURN_ERROR;
			}
			*nBytesReceived = pNetBuf->p->len;
			memcpy(pData, pNetBuf->p->payload, *nBytesReceived);
		}
		netbuf_delete(pNetBuf);
	}else if(tError == ERR_TIMEOUT)
	{
		return SE_RETURN_TIMEOUT;
	}else{
		SeErrorPrint("Network receive fail!");
		return SE_RETURN_ERROR;
	}

#endif

#ifdef CONFIG_USING_LINUX

#endif

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterSend(SeNetworkConnectionHandle tNetworkConnectionHandle, SeUInt8* pData, SeUInt16 nBytesToSend, SeUInt16* nBytesSent)
{
#ifdef CONFIG_USING_LWIP
	if(tNetworkConnectionHandle.tNetworkConnectionType == SeNetworkTcpConnection)
	{
		netconn_write(tNetworkConnectionHandle.pConnection, pData, nBytesToSend, NETCONN_COPY);
	}else if(tNetworkConnectionHandle.tNetworkConnectionType == SeNetworkUdpConnection)
	{
		struct netbuf* pNetBuf  = netbuf_new();
		netbuf_ref(pNetBuf, pData, nBytesToSend);
		netconn_send(tNetworkConnectionHandle.pConnection, pNetBuf);
		netbuf_delete(pNetBuf);
	}
	*nBytesSent = nBytesToSend;
#endif

#ifdef CONFIG_USING_LINUX

#endif

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterReceiveFrom(SeNetworkConnectionHandle tNetworkConnectionHandle, SeUInt8* pData, SeUInt16 nBytesToReceive, SeUInt16* nBytesReceived, SeIpAddress* pRemoteIp, SeIpPort* pRemotePort)
{
#ifdef CONFIG_USING_LWIP
	if(tNetworkConnectionHandle.tNetworkConnectionType != SeNetworkUdpConnection)
	{
		SeErrorPrint("Not supported operation!");
		return SE_RETURN_ERROR;
	}

	struct netbuf* pNetBuf;
	err_t tError;

	*nBytesReceived = 0;

	tError = netconn_recv(tNetworkConnectionHandle.pConnection, &pNetBuf);
	if(tError == ERR_OK)
	{
		do{
			struct ip_addr* pAddr = netbuf_fromaddr(pNetBuf);
			SeUInt16 nPort = netbuf_fromport(pNetBuf);
			void* pDataPtr = SeNull;
			SeUInt16 nDataLen;
			netbuf_data(pNetBuf, &pDataPtr, &nDataLen);
			memcpy((pData + *nBytesReceived), pDataPtr, nDataLen);
			pRemoteIp->aIpAddress[0] = (SeUInt8)pAddr->addr;
			pRemoteIp->aIpAddress[1] = (SeUInt8)(pAddr->addr >> 8);
			pRemoteIp->aIpAddress[2] = (SeUInt8)(pAddr->addr >> 16);
			pRemoteIp->aIpAddress[3] = (SeUInt8)(pAddr->addr >> 24);
			*pRemotePort = nPort;
			*nBytesReceived += nDataLen;
		}while(netbuf_next(pNetBuf) >= 0);
		netbuf_delete(pNetBuf);
	}else if(tError == ERR_TIMEOUT)
	{
		return SE_RETURN_TIMEOUT;
	}else{
		SeErrorPrint("Network receive fail!");
		return SE_RETURN_ERROR;
	}
#endif

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAdapterSendTo(SeNetworkConnectionHandle tNetworkConnectionHandle, SeUInt8* pData, SeUInt16 nBytesToSend, SeUInt16* nBytesSent, SeIpAddress tRemoteIp, SeIpPort tRemotePort)
{
#ifdef CONFIG_USING_LWIP
	if(tNetworkConnectionHandle.tNetworkConnectionType != SeNetworkUdpConnection)
	{
		SeErrorPrint("Not supported operation!");
		return SE_RETURN_ERROR;
	}

	err_t tError;
	struct netbuf* pNetBuf  = netbuf_new();
	netbuf_ref(pNetBuf, pData, nBytesToSend);

	struct ip_addr tIpAddr;
	SeUInt16 nPort;

	IP4_ADDR(&tIpAddr, tRemoteIp.aIpAddress[0], tRemoteIp.aIpAddress[1], tRemoteIp.aIpAddress[2], tRemoteIp.aIpAddress[3]);
	nPort = tRemotePort;
	netconn_connect(tNetworkConnectionHandle.pConnection, &tIpAddr, nPort);
	pNetBuf->addr.addr = 0;
	netconn_send(tNetworkConnectionHandle.pConnection, pNetBuf);
	netbuf_delete(pNetBuf);

#endif

	return SE_RETURN_OK;
}
