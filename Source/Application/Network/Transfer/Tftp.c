/*
 * Tftp.c
 *
 *  Created on: 2016年3月14日
 *      Author: seeing
 */

#include "Tftp.h"
#include <stdio.h>
#include <string.h>

typedef enum
{
	SeTftpPacketTypeReadRequest = 1,
	SeTftpPacketTypeWriteRequest,
	SeTftpPacketTypeData,
	SeTftpPacketTypeAck,
	SeTftpPacketTypeError,
}SeTftpPacketType;

#define TFTP_DATA_PLAYLOAD_LENGTH			512
#define TFTP_MAX_DATA_FRAME_LENGTH			(TFTP_DATA_PLAYLOAD_LENGTH + 2 + 2)

static SeUInt8 aTftpBuffer[TFTP_MAX_DATA_FRAME_LENGTH] = {0};
static SeUInt8 aTftpTempBuffer[TFTP_MAX_DATA_FRAME_LENGTH] = {0};

static void SeTftpClientMakeRequest(SeTftpPacketType tRequestType, SeString sFileName, SeTftpFileType tFileType, SeUInt8* pBuffer, SeUInt16* pLength)
{
	SeUInt16 iCursor = 0;
	SeChar sFileType[SE_BUFFER_SIZE_16] = {0};

	if(tFileType == SeTftpFileTypeNetAscII)
	{
		strcat(sFileType, "netascii");
	}else{
		strcat(sFileType, "octet");
	}

	pBuffer[iCursor++] = 0;
	pBuffer[iCursor++] = (SeUInt8)tRequestType;

	strncpy(&pBuffer[iCursor], sFileName, strlen(sFileName));
	iCursor += strlen(sFileName);

	pBuffer[iCursor++] = 0;

	strncpy(&pBuffer[iCursor], sFileType, strlen(sFileType));
	iCursor += strlen(sFileType);

	pBuffer[iCursor++] = 0;

	*pLength = iCursor;

	return;
}

static void SeTftpClientMakeAck(SeUInt16 hBlockCode, SeUInt8* pBuffer, SeUInt16* pLength)
{
    SeUInt16 iCursor = 0;

	pBuffer[iCursor++] = 0;
	pBuffer[iCursor++] = (SeUInt8)SeTftpPacketTypeAck;
	pBuffer[iCursor++] = (SeUInt8)(hBlockCode >> 8);
	pBuffer[iCursor++] = (SeUInt8)hBlockCode;

	*pLength = iCursor;

	return;
}

static void SeTftpClientMakeData(SeUInt16 hBlockCode, SeUInt8* pData, SeUInt16 nSize, SeUInt8* pBuffer, SeUInt16* pLength)
{
    SeUInt16 iCursor = 0;

	pBuffer[iCursor++] = 0;
	pBuffer[iCursor++] = (SeUInt8)SeTftpPacketTypeData;
	pBuffer[iCursor++] = (SeUInt8)(hBlockCode >> 8);
	pBuffer[iCursor++] = (SeUInt8)hBlockCode;

	memcpy(&pBuffer[iCursor], pData, nSize);
	iCursor += nSize;

	*pLength = iCursor;

	return;
}

static SeUInt16 SeTftpClientGetBlockCode(SeUInt8* pBuffer, SeUInt16 nSize)
{
	return (((SeUInt16)aTftpBuffer[2] << 8)|aTftpBuffer[3]);
}

static SeBool SeTftpClientAckIsError(SeUInt8* pBuffer, SeUInt16 nSize)
{
	if(pBuffer[1] == SeTftpPacketTypeError)
	{
		return SeTrue;
	}
	return SeFalse;
}

static SeBool SeTftpClientAckIsData(SeUInt8* pBuffer, SeUInt16 nSize)
{
	if(pBuffer[1] == SeTftpPacketTypeData)
	{
		return SeTrue;
	}
	return SeFalse;
}

SeInt8 SeTftpClientGetFile(SeTftpClientDescription tTftpClientDescription, SeString sFileName, SeTftpFileType tTftpFileType, SeTftpClientGetFileOperation tTftpClientGetFileOperation, SeString sDescription, SeUpdateApplicationType tUpdateApplicationType)
{
	SeNetworkConnectionHandle tServerConnection;
	SeUInt16 nPackLen;
	SeUInt16 nCount;
	SeUInt32 iPosition = 0;

	if(SeNetworkAdapterCreate(&tServerConnection, SeNetworkUdpConnection) != SE_RETURN_OK)
	{
		SeErrorPrint("Create connection fail!");
		return SE_RETURN_ERROR;
	}

	SeIpAddress tLocalIp;
	tLocalIp.aIpAddress[0] = 0;
	tLocalIp.aIpAddress[1] = 0;
	tLocalIp.aIpAddress[2] = 0;
	tLocalIp.aIpAddress[3] = 0;
	SeIpPort tLocalPort = tTftpClientDescription.tServerPort;
	if(SeNetworkAdapterBind(tServerConnection, tLocalIp, tLocalPort) != SE_RETURN_OK)
	{
		SeErrorPrint("Bind local port error!");
		SeNetworkAdapterClose(tServerConnection);
		return SE_RETURN_ERROR;
	}

	if(tTftpClientGetFileOperation.fpSeTftpClientOpen(sDescription, tUpdateApplicationType) != SE_RETURN_OK)
	{
		SeErrorPrint("Open local file error!");
		SeNetworkAdapterClose(tServerConnection);
		return SE_RETURN_ERROR;
	}

	SeTftpClientMakeRequest(SeTftpPacketTypeReadRequest, sFileName, tTftpFileType, aTftpBuffer, &nPackLen);

	if(SeNetworkAdapterSendTo(tServerConnection, aTftpBuffer, nPackLen, &nCount, tTftpClientDescription.tServerIp, tTftpClientDescription.tServerPort) != SE_RETURN_OK)
	{
		SeErrorPrint("Send read request fail!");
		tTftpClientGetFileOperation.fpSeTftpClientClose();
		SeNetworkAdapterClose(tServerConnection);
		return SE_RETURN_ERROR;
	}

	SeNetworkAdapterClose(tServerConnection);

	if(SeNetworkAdapterCreate(&tServerConnection, SeNetworkUdpConnection) != SE_RETURN_OK)
	{
		SeErrorPrint("Create connection fail!");
		tTftpClientGetFileOperation.fpSeTftpClientClose();
		return SE_RETURN_ERROR;
	}

	if(SeNetworkAdapterBind(tServerConnection, tLocalIp, tLocalPort) != SE_RETURN_OK)
	{
		SeErrorPrint("Bind local port error!");
		tTftpClientGetFileOperation.fpSeTftpClientClose();
		SeNetworkAdapterClose(tServerConnection);
		return SE_RETURN_ERROR;
	}

	SeNetworkAdapterSetBlock(&tServerConnection, SeFalse, 5000);

	SeInt8 nRetry = 0;

	SeHoldLinePrintStart();

	while(SeTrue)
	{
		SeInt8 nResult;
		SeIpAddress tRemoteIp;
		SeIpPort tRemotePort;
		memset(aTftpBuffer, 0, sizeof(aTftpBuffer));
		nResult = SeNetworkAdapterReceiveFrom(tServerConnection, aTftpBuffer, sizeof(aTftpBuffer), &nCount, &tRemoteIp, &tRemotePort);
		if(nResult == SE_RETURN_ERROR)
		{
			SeErrorPrint("Receive fail!");
			tTftpClientGetFileOperation.fpSeTftpClientClose();
			SeNetworkAdapterClose(tServerConnection);
			return SE_RETURN_ERROR;
		}else if(nResult == SE_RETURN_TIMEOUT)
		{
			nRetry++;
			if(nRetry > 3)
			{
				SeErrorPrint("Receive timeout!");
				tTftpClientGetFileOperation.fpSeTftpClientClose();
				SeNetworkAdapterClose(tServerConnection);
				return SE_RETURN_ERROR;
			}
			continue;
		}else{
			nRetry = 0;
		}

		if(nCount == 0)
		{
			SeErrorPrint("Receive data is not correct!");
			tTftpClientGetFileOperation.fpSeTftpClientClose();
			SeNetworkAdapterClose(tServerConnection);
			return SE_RETURN_ERROR;
		}

		if(SeTftpClientAckIsError(aTftpBuffer, nCount) == SeTrue)
		{
			SeErrorPrint("Send receive data is error ack!");
			tTftpClientGetFileOperation.fpSeTftpClientClose();
			SeNetworkAdapterClose(tServerConnection);
			return SE_RETURN_ERROR;
		}

		if(SeTftpClientAckIsData(aTftpBuffer, nCount) == SeTrue)
		{
			/*
			 * write data and send ack
			 */
			SeUInt16 hBlockCode = SeTftpClientGetBlockCode(aTftpBuffer, nCount);
			SeUInt16 nSent;
			SeUInt16 nReceived;

			nReceived = nCount;

			SeTftpClientMakeAck(hBlockCode, aTftpTempBuffer, &nCount);

			if(SeNetworkAdapterSendTo(tServerConnection, aTftpTempBuffer, nCount, &nSent, tRemoteIp, tRemotePort) != SE_RETURN_OK)
			{
				SeErrorPrint("Send ack fail!");
				tTftpClientGetFileOperation.fpSeTftpClientClose();
				SeNetworkAdapterClose(tServerConnection);
				return SE_RETURN_ERROR;
			}

			if(tTftpClientGetFileOperation.fpSeTftpClientWrite(iPosition, &aTftpBuffer[4], (nReceived - 2 - 2)) != SE_RETURN_OK)
			{
				SeErrorPrint("Write file fail!");
				tTftpClientGetFileOperation.fpSeTftpClientClose();
				SeNetworkAdapterClose(tServerConnection);
				return SE_RETURN_ERROR;
			}
			iPosition += (nReceived - 2 - 2);

			SeHoldLinePrint("Tftp received file size:%d", iPosition);

			if(nReceived < TFTP_MAX_DATA_FRAME_LENGTH)
			{
				break;
			}
		}
	}

	SeHoldLinePrintFinish();

	SeNormalPrint("Total receive %d bytes.", iPosition);

	tTftpClientGetFileOperation.fpSeTftpClientClose();

	SeNetworkAdapterClose(tServerConnection);

	return SE_RETURN_OK;
}

SeInt8 SeTftpClientPutFile(SeTftpClientDescription tTftpClientDescription, SeString sFileName, SeTftpFileType tTftpFileType, SeTftpClientPutFileOperation tTftpClientPutFileOperation, SeString sDescription, SeUpdateApplicationType tUpdateApplicationType)
{

	return SE_RETURN_OK;
}
