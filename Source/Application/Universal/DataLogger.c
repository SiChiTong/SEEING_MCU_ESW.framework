/*
 * DataLogger.c
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#include "DataLogger.h"

#define SE_FPGA_MEASURE_PAGE_ADDRESS 0x01
#define SE_FPGA_DATALOGGER_PAGE_ADDRESS 0x0A
#define SE_FPGA_AD9265_PAGE_ADDRESS 0x0B

SeInt8 SeNetworkAcquisitionAd9265Prepare(SeUInt16 nSampleTime)
{
	SeUInt16 hAddr;
	SeInt8 nTimeout = 100;

	/*
	 * configure input range and impedance
	 */
	SeFpgaAd9265MeasureRangeAndImpedanceSelectRegister tAd9265RangeAndImpedance;
	tAd9265RangeAndImpedance.Byte = 0x00;
	tAd9265RangeAndImpedance.Bit.Range = SE_BIT_HIGH;
	tAd9265RangeAndImpedance.Bit.Impedance = SE_BIT_LOW;
	hAddr = ((SeUInt16)SE_FPGA_AD9265_PAGE_ADDRESS << 8)|SeFpgaAd9265MeasureRangeAndImpedanceSelectRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tAd9265RangeAndImpedance, sizeof(tAd9265RangeAndImpedance));

	/*
	 * delay for relay switch over
	 */
	SeDelayMs(10);

	SeFpgaAd9265ControlRegister tAd9625Control;
	tAd9625Control.Byte = 0x00;
	tAd9625Control.Bit.Disable = SE_BIT_LOW;
	hAddr = ((SeUInt16)SE_FPGA_AD9265_PAGE_ADDRESS << 8)|SeFpgaAd9265ControlRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tAd9625Control, sizeof(tAd9625Control));

	SeFpgaAd9265AcquisitionTimeRegister tAcqTime;
	tAcqTime = nSampleTime;
	hAddr = ((SeUInt16)SE_FPGA_AD9265_PAGE_ADDRESS << 8)|SeFpgaAd9265AcquisitionTimeRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tAcqTime, sizeof(tAcqTime));

	SeFpgaAd9265AcquisitionControlRegister tAcqControl;
	tAcqControl.Bit.Start = SE_BIT_HIGH;
	hAddr = ((SeUInt16)SE_FPGA_AD9265_PAGE_ADDRESS << 8)|SeFpgaAd9265AcquisitionControlRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tAcqControl, sizeof(tAcqControl));

	while(nTimeout--)
	{
		SeFpgaAd9265AcquisitionStatusRegister tAcqState;
		tAcqState.Byte = 0x00;
		hAddr = ((SeUInt16)SE_FPGA_AD9265_PAGE_ADDRESS << 8)|SeFpgaAd9265AcquisitionStatusRegisterAddress;
		SeFpgaReadDatas(hAddr, (SeUInt8*)&tAcqState, sizeof(tAcqState));
		if(tAcqState.Bit.Done == SE_BIT_HIGH)
		{
			SeNormalPrint(">>>Acquisition ok...");
			break;
		}
		SeDelayMs(100);
	}
	if(nTimeout <= 0)
	{
		SeErrorPrint("Acquisition timeout...");
		return SE_RETURN_ERROR;
	}
	/*
	SeFpgaMeasureChannelEnableRegister tMeasureChEna;
	tMeasureChEna.Byte = 0x00;
	tMeasureChEna.Bit.Enable = SE_BIT_HIGH;
	hAddr = ((SeUInt16)SE_FPGA_MEASURE_PAGE_ADDRESS << 8)|SeFpgaMeasureChannelEnableRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tMeasureChEna, sizeof(tMeasureChEna));

	SeFpgaMeasurePeriodRegister tMeasurePeriod;
	tMeasurePeriod = nSampleTime;			//1ms
	hAddr = ((SeUInt16)SE_FPGA_MEASURE_PAGE_ADDRESS << 8)|SeFpgaMeasurePeriodRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tMeasurePeriod, sizeof(tMeasurePeriod));

	SeFpgaMeasureStartRegister tMeasureStart;
	tMeasureStart.Byte = 0x00;
	tMeasureStart.Bit.Start = SE_BIT_HIGH;
	hAddr = ((SeUInt16)SE_FPGA_MEASURE_PAGE_ADDRESS << 8)|SeFpgaMeasureStartRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tMeasureStart, sizeof(tMeasureStart));

	while(nTimeout--)
	{
		SeFpgaMeasureStateRegister tMeasureState;
		tMeasureState.Byte = 0x00;
		hAddr = ((SeUInt16)SE_FPGA_MEASURE_PAGE_ADDRESS << 8)|SeFpgaMeasureStateRegisterAddress;
		SeFpgaReadDatas(hAddr, (SeUInt8*)&tMeasureState, sizeof(tMeasureState));
		if(tMeasureState.Bit.State == SE_BIT_HIGH)
		{
			SeNormalPrint(">>>Measure pass...");
			break;
		}
		SeDelayMs(100);
	}
	if(nTimeout <= 0)
	{
		SeErrorPrint("Measure timeout...");
		return SE_RETURN_ERROR;
	}

	SeFpgaDataLoggerAcquisitionStartRegister tDataLoggerStart;
	tDataLoggerStart.Byte = 0x00;
	tDataLoggerStart.Bit.Start = SE_BIT_HIGH;
	hAddr = ((SeUInt16)SE_FPGA_DATALOGGER_PAGE_ADDRESS << 8)|SeFpgaDataLoggerAcquisitionStartRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tDataLoggerStart, sizeof(tDataLoggerStart));
*/

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAcquisitionAd9265GetDataSize(SeUInt32* nTotalSize)
{
	SeUInt16 hAddr;

	SeFpgaDataLoggerByteCountRegister tCount;

	hAddr = ((SeUInt16)SE_FPGA_DATALOGGER_PAGE_ADDRESS << 8)|SeFpgaDataLoggerByteCountRegisterAddress;
	SeFpgaReadDatas(hAddr, (SeUInt8*)&tCount, sizeof(tCount));

	*nTotalSize = (((SeUInt32)tCount)*sizeof(SeFpgaDataLoggerAcquisitionBufferRegister));

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAcquisitionAd9265ReadData(SeUInt32 iSeekPosition, SeUInt8* pData, SeUInt16 nBytesToRead, SeUInt16* nBytesReaded)
{
	SeFpgaDataLoggerAcquisitionBufferRegister tData;
	SeUInt16 hAddr = ((SeUInt16)SE_FPGA_DATALOGGER_PAGE_ADDRESS << 8)|SeFpgaDataLoggerAcquisitionBufferRegisterAddress;

	*nBytesReaded = 0;

	while(*nBytesReaded < nBytesToRead)
	{
		SeFpgaReadDatas(hAddr, (SeUInt8*)&tData, sizeof(tData));
		*(pData + *nBytesReaded) = tData.aBuffer[0];
		*(pData + *nBytesReaded + 1) = tData.aBuffer[1];
		*nBytesReaded += sizeof(tData);
	}

	return SE_RETURN_OK;
}

SeInt8 SeNetworkAcquisitionAd9265Finish(void)
{
	SeUInt16 hAddr;

	SeFpgaDataLoggerAcquisitionStatusRegister tDataLoggerStatus;
	tDataLoggerStatus.Byte = 0x00;
	hAddr = ((SeUInt16)SE_FPGA_DATALOGGER_PAGE_ADDRESS << 8)|SeFpgaDataLoggerAcquisitionStatusRegisterAddress;
	SeFpgaReadDatas(hAddr, (SeUInt8*)&tDataLoggerStatus, sizeof(tDataLoggerStatus));
	if(tDataLoggerStatus.Bit.ReadOver == SE_BIT_HIGH)
	{
		SeNormalPrint(">>>Acquisition success...");
	}else{
		SeErrorPrint(">>>Acquisition fail...");
	}

	SeFpgaAd9265ControlRegister tAd9625Control;
	tAd9625Control.Byte = 0x00;
	tAd9625Control.Bit.Disable = SE_BIT_HIGH;
	hAddr = ((SeUInt16)SE_FPGA_AD9265_PAGE_ADDRESS << 8)|SeFpgaAd9265ControlRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tAd9625Control, sizeof(tAd9625Control));
/*
	SeFpgaMeasureChannelEnableRegister tMeasureChEna;
	tMeasureChEna.Byte = 0x00;
	tMeasureChEna.Bit.Enable = SE_BIT_LOW;
	hAddr = ((SeUInt16)SE_FPGA_MEASURE_PAGE_ADDRESS << 8)|SeFpgaMeasureChannelEnableRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tMeasureChEna, sizeof(tMeasureChEna));
*/
	SeFpgaDataLoggerAcquisitionStartRegister tDataLoggerStart;
	tDataLoggerStart.Byte = 0x00;
	tDataLoggerStart.Bit.Start = SE_BIT_LOW;
	hAddr = ((SeUInt16)SE_FPGA_DATALOGGER_PAGE_ADDRESS << 8)|SeFpgaDataLoggerAcquisitionStartRegisterAddress;
	SeFpgaWriteDatas(hAddr, (SeUInt8*)&tDataLoggerStart, sizeof(tDataLoggerStart));

	return SE_RETURN_OK;
}

