/*
 * Acquisition.h
 *
 *  Created on: 2016年3月5日
 *      Author: seeing
 */

#ifndef _APPLICATION_NETWORK_TRANSFER_ACQUISITION_H_
#define _APPLICATION_NETWORK_TRANSFER_ACQUISITION_H_

#include "../../../Global/Include.h"

typedef enum
{
	SeNetworkAcquisitionPacketTypeInfo = 0x01,
	SeNetworkAcquisitionPacketTypeDataSize,
	SeNetworkAcquisitionPacketTypeAck,
}SeNetworkAcquisitionPacketTypes;

typedef enum
{
	SeNetworkAcquisitionErrorTypeSuccess = 0x01,
}SeNetworkAcquisitionErrorTypes;

typedef struct
{
	SeUInt16 hLeadWord;
	SeUInt8 hPacketType;
}SeNetworkAcquisitionManagePacketHead;

typedef enum
{
	SeNetworkAcquisitionData8Bit,
	SeNetworkAcquisitionData12Bit,
	SeNetworkAcquisitionData16Bit,
	SeNetworkAcquisitionData24Bit,
	SeNetworkAcquisitionData32Bit
}SeNetworkAcquisitionDataWidth;

typedef SeUInt32 SeNetworkAcquisitionDataRate;

typedef struct
{
	SeNetworkAcquisitionDataWidth tDataWidth;
	SeNetworkAcquisitionDataRate tDataRate;
	SeUInt16 nSampleTime;
}SeNetworkAcquisitionInformation;

typedef struct
{
	SeNetworkAcquisitionErrorTypes tError;
}SeNetworkAcquisitionAck;

typedef union
{
	SeUInt8 aDataSize[4];
	SeUInt32 nDataSize;
}SeNetworkAcquisitionDataSize;

typedef struct
{
	SeInt8 (*fpSeNetworkAcquisitionPrepare)(SeUInt16 nSampleTime);
	SeInt8 (*fpSeNetworkAcquisitionGetDataSize)(SeUInt32* nTotalSize);
	SeInt8 (*fpSeNetworkAcquisitionReadData)(SeUInt32 iSeekPosition, SeUInt8* pData, SeUInt16 nBytesToRead, SeUInt16* nBytesReaded);
	SeInt8 (*fpSeNetworkAcquisitionFinish)(void);
}SeNetworkAcquisitionOperation;

typedef struct
{
	SeIpAddress tServerIp;
	SeIpPort tServerPort;
	SeNetworkAcquisitionInformation tNetworkAcquisitionInformation;
	SeNetworkAcquisitionOperation tNetworkAcquisitionOperation;
}SeNetworkAcquisitionDescription;

SeInt8 SeNetworkAcquisitionSessionCreate(SeNetworkAcquisitionDescription tNetworkAcquisitionDescription);

#endif /* _APPLICATION_NETWORK_TRANSFER_ACQUISITION_H_ */
