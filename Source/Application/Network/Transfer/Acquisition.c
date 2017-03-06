/*
 * Acquisition.c
 *
 *  Created on: 2016年3月5日
 *      Author: seeing
 */

#include "Acquisition.h"

#define NETWORK_ACQUISITION_MANAGE_BUFFER_SIZE						100
#define NETWORK_ACQUISITION_MANAGE_PACKET_LEAD					0x6688

static SeUInt8 aManageBuffer[NETWORK_ACQUISITION_MANAGE_BUFFER_SIZE] = {0};
static SeUInt8 aTransmitBuffer[CONFIG_NETWORK_ACQUISITION_DATA_TRANSMIT_SIZE] = {0};

SeInt8 SeNetworkAcquisitionSessionCreate(SeNetworkAcquisitionDescription tNetworkAcquisitionDescription)
{
	SeNetworkAcquisitionManagePacketHead tManagePacketHead;
	SeNetworkAcquisitionAck tManagePacketAck;

	SeNetworkConnectionHandle tHandle;

	SeUInt16 iManageBufferPos = 0;
	SeUInt16 nReaded = 0;
	SeUInt32 nTotalSize;
	SeUInt32 nTotalSent = 0;
	SeUInt16 nSent = 0;
	SeInt8 iCount;
	SeUInt8 nPass;

	SeNormalPrint("Ready to launch acquisition processing...");

	SeNetworkAcquisitionOperation* pOperation = &(tNetworkAcquisitionDescription.tNetworkAcquisitionOperation);

	/*
	 * create a new manage session
	 */
	if(SeNetworkAdapterCreate(&tHandle, SeNetworkTcpConnection) != SE_RETURN_OK)
	{
		SeErrorPrint("Create manager connection fail!");
		return SE_RETURN_ERROR;
	}
	SeNormalPrint(">>>Create manage session success!");

	/*
	 * connect to server manage session
	 */
	if(SeNetworkAdapterConnect(tHandle, tNetworkAcquisitionDescription.tServerIp, tNetworkAcquisitionDescription.tServerPort) != SE_RETURN_OK)
	{
		SeErrorPrint("Connect to server fail!");
		SeNetworkAdapterClose(tHandle);
		return SE_RETURN_ERROR;
	}
	SeNormalPrint(">>>Connect to server success!");

	SeNetworkAdapterSetBlock(&tHandle, SeFalse, 1000);

	/*
	 * send information packet
	 */
	memset(aManageBuffer, 0, sizeof(aManageBuffer));
	tManagePacketHead.hLeadWord = NETWORK_ACQUISITION_MANAGE_PACKET_LEAD;
	tManagePacketHead.hPacketType = SeNetworkAcquisitionPacketTypeInfo;
	memcpy(aManageBuffer, &tManagePacketHead, sizeof(SeNetworkAcquisitionManagePacketHead));
	iManageBufferPos += sizeof(SeNetworkAcquisitionManagePacketHead);
	memcpy((aManageBuffer + iManageBufferPos), &(tNetworkAcquisitionDescription.tNetworkAcquisitionInformation), sizeof(SeNetworkAcquisitionInformation));
	iManageBufferPos += sizeof(SeNetworkAcquisitionInformation);
	if(SeNetworkAdapterSend(tHandle, aManageBuffer, iManageBufferPos, &nSent) != SE_RETURN_OK)
	{
		SeErrorPrint("Send information to server fail!");
		SeNetworkAdapterClose(tHandle);
		return SE_RETURN_ERROR;
	}
	SeNormalPrint(">>>Send data information success!");

	if(pOperation->fpSeNetworkAcquisitionPrepare(tNetworkAcquisitionDescription.tNetworkAcquisitionInformation.nSampleTime) != SE_RETURN_OK)
	{
		SeErrorPrint("Local acquisition prepare error!");
		SeNetworkAdapterClose(tHandle);
		return SE_RETURN_ERROR;
	}
	SeNormalPrint(">>>Local ADC device prepared!");

	if(pOperation->fpSeNetworkAcquisitionGetDataSize(&nTotalSize) != SE_RETURN_OK)
	{
		SeErrorPrint("Network acquisition prepare error!");
		SeNetworkAdapterClose(tHandle);
		pOperation->fpSeNetworkAcquisitionFinish();
		return SE_RETURN_ERROR;
	}
	SeNormalPrint(">>>Ready to transmit %ld Byte data!", nTotalSize);

	SeNetworkAcquisitionDataSize tAcquisitionDataSize;
	tAcquisitionDataSize.nDataSize = nTotalSize;
	memset(aManageBuffer, 0, sizeof(aManageBuffer));
	iManageBufferPos = 0;
	tManagePacketHead.hLeadWord = NETWORK_ACQUISITION_MANAGE_PACKET_LEAD;
	tManagePacketHead.hPacketType = SeNetworkAcquisitionPacketTypeDataSize;
	memcpy(aManageBuffer, &tManagePacketHead, sizeof(SeNetworkAcquisitionManagePacketHead));
	iManageBufferPos += sizeof(SeNetworkAcquisitionManagePacketHead);
	memcpy((aManageBuffer + iManageBufferPos), &tAcquisitionDataSize, sizeof(SeNetworkAcquisitionDataSize));
	iManageBufferPos += sizeof(SeNetworkAcquisitionDataSize);
	if(SeNetworkAdapterSend(tHandle, aManageBuffer, iManageBufferPos, &nSent) != SE_RETURN_OK)
	{
		SeErrorPrint("Send data size to server fail!");
		SeNetworkAdapterClose(tHandle);
		pOperation->fpSeNetworkAcquisitionFinish();
		return SE_RETURN_ERROR;
	}
	SeNormalPrint(">>>Send data size success!");

	/*
	 * start transmit
	 */
	SeFloat fPercentage = 0.00f;
	SeProgressStart("Transmitting data...");
	while(nTotalSent < nTotalSize)
	{
		SeUInt32 iReadPos = nTotalSent;
		SeUInt16 nReaded = 0;
		SeUInt16 nSent = 0;
		SeUInt16 nWant = CONFIG_NETWORK_ACQUISITION_DATA_TRANSMIT_SIZE;
		SeUInt32 nRemain = nTotalSize - nTotalSent;
		if(nRemain < sizeof(aTransmitBuffer))
			nWant = nRemain;

		if(pOperation->fpSeNetworkAcquisitionReadData(iReadPos, aTransmitBuffer, nWant, &nReaded) != SE_RETURN_OK)
		{
			SeNetworkAdapterClose(tHandle);
			pOperation->fpSeNetworkAcquisitionFinish();
			return SE_RETURN_ERROR;
		}
		if(SeNetworkAdapterSend(tHandle, aTransmitBuffer, nReaded, &nSent) != SE_RETURN_OK)
		{
			SeNetworkAdapterClose(tHandle);
			pOperation->fpSeNetworkAcquisitionFinish();
			return SE_RETURN_ERROR;
		}
		nTotalSent += nSent;

		fPercentage = ((SeFloat)nTotalSent/ (SeFloat)nTotalSize);
		fPercentage = fPercentage*100;

		SeProgressPrint((SeUInt8)fPercentage, 1);
	}
	SeProgressFinish();

	SeNormalPrint(">>>Transmission success!");

	pOperation->fpSeNetworkAcquisitionFinish();
	SeNetworkAdapterClose(tHandle);

	SeNormalPrint("Finish!");

	return SE_RETURN_OK;
}
