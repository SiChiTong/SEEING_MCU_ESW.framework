/*
 * Handle.c
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#include "Handle.h"

static SeBool SeParameterIsAskForHelp(void* pParameterItem)
{
	SeCommandParameterInf* pItem = (SeCommandParameterInf*)pParameterItem;
	if(pItem->nParameterLength != 1)
	{
		return SeFalse;
	}
	if(strncmp(pItem->sParameter, "?", pItem->nParameterLength) != 0)
	{
		return SeFalse;
	}
	return SeTrue;
}


SeInt8 SeHelpHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: help({command})";
	SeNormalPrint("%s", sUsage);
	return SE_RETURN_OK;
}

SeInt8 SeFpgaReadHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: fpga read({address},{count})";

	SeUInt16 hStartAddress = 0;
	SeInt16 iParamIndex = 0;
	SeInt16 nByteToRead = 0;
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeUInt8 aBytes[SE_BUFFER_SIZE_16] = {0};

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
			}else{
				hStartAddress = strtoul(sTemp, SeNull, 16);
			}
		}else{
			nByteToRead = strtoul(sTemp, SeNull, 10);
		}

	}

	if(SeFpgaRegisterRead(hStartAddress, aBytes, nByteToRead) == SE_RETURN_ERROR)
	{
		SeNormalPrint("fpga read error!");
		return SE_RETURN_ERROR;
	}

	for(iParamIndex = 0; iParamIndex < nByteToRead; iParamIndex++)
	{
		SeChar sTemp[8] = {0};
		sprintf(sTemp, "%02x", aBytes[iParamIndex]);
		strcat(sResult, sTemp);
		if(iParamIndex != nByteToRead - 1)
		{
			strcat(sResult, ",");
		}
	}

	return SE_RETURN_OK;
}

SeInt8 SeFpgaWriteHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: fpga write({address},{count},{<data>})";

	SeUInt16 hStartAddress = 0;
	SeInt16 iParamIndex = 0;
	SeInt16 nByteToWrite = 0;
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeUInt8 aBytes[SE_BUFFER_SIZE_16] = {0};
	SeInt16 iDataIndex = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
			}else{
				hStartAddress = strtoul(sTemp, SeNull, 16);
			}
		}else if(iParamIndex == 1){
			nByteToWrite = strtoul(sTemp, SeNull, 10);
		}else{
			aBytes[iDataIndex++] = strtoul(sTemp, SeNull, 16);
		}
	}

	if(nByteToWrite != iDataIndex)
	{
		SeNormalPrint("check the parameters!");
		return SE_RETURN_ERROR;
	}

	if(SeFpgaRegisterWrite(hStartAddress, aBytes, nByteToWrite) == SE_RETURN_ERROR)
	{
		SeNormalPrint("fpga write error!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("fpga write done!", aBytes);
	return SE_RETURN_OK;
}

SeInt8 SeFpgaFrequencyMeasureHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: frequency measure({<-option>},<vref>,{<time>},{<timeout>})\n \
			option:(d)\n  \
			    default none,not show some result which use for debug.\n \
                d: show the some result use for debug.\n \
            vref:(0-5000) mV  reference voltage.\n \
            time: (1-4000) ms   sample time, default 300 ms.\n\
            timeout: (1-10000) ms  measure timeout time,default 3000 ms.";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeInt16 iParamIndex = 0;
	SeInt16 iBaseParamIndex = 0;
	SeUInt8 nChannel = 1;
	SeUInt16 nTime = 300;				// default 300ms
	SeUInt32 nTimeout = 3000;		// default 3000ms
	SeDouble dVerf;
	SeBool blDebug = SeFalse;
	SeFpgaFrequency tFrequency;

	SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, 0);
	memset(sTemp, 0, sizeof(sTemp));
	strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);

	if(SeParameterIsAskForHelp(pItem))
	{
		SeNormalPrint("%s", sUsage);
		return SE_RETURN_OK;
	} else {
		if(sTemp[0] == '-')
		{
			if(sTemp[1] == 'd')
			{
				blDebug = SeTrue;
				iBaseParamIndex++;
			}
		}
	}

	for(iParamIndex = iBaseParamIndex; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
	    pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == iBaseParamIndex) {
			dVerf = atof(sTemp);
		} else if(iParamIndex == 1 + iBaseParamIndex) {
			nTime = strtoul(sTemp, SeNull, 10);;
		} else if(iParamIndex == 2 + iBaseParamIndex) {
			nTimeout =strtoul(sTemp, SeNull, 10);;
		}
	}

	SeFpgaEnableChannel(nChannel);
	SeFpgaConfigTime(nChannel, nTime);
	SeFpgaStartMeasure(nChannel);
	SeDelayMs(nTime);
	
	if(SeFpgaMeasureIsTimeout(nChannel, nTimeout))
	{
		SeNormalPrint("frequency measure error!");
		return SE_RETURN_ERROR;
	}

	if(SeFpgaGetFrequencyMeasureResult(nChannel,  &tFrequency) == SE_RETURN_ERROR)
	{
		SeNormalPrint("frequency measure get result error!");
		return SE_RETURN_ERROR;
	}

	if(blDebug)
	{
		SeNormalPrint("ACK(%f Hz,clk=%u Hz,freq_cnt=%llu, div=%u,x_sum=%llu,y_sum=%llu,xy_sum=%llu,xx_sum=%llu",
				tFrequency.dFrequency,
				tFrequency.nClock, tFrequency.nCycleCount, tFrequency.nDivision,
				tFrequency.nX, tFrequency.nY, tFrequency.nXY, tFrequency.nXX);
	} else {
		SeNormalPrint("ACK(%f Hz)", tFrequency.dFrequency);
	}

	return SE_RETURN_OK;
}

SeInt8 SeFpgaAmplitudeMeasureHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: amplitude measure(<time>,<division>,<threshold>)";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeInt16 iParamIndex = 0;
	SeUInt16 uTime = 1;
	SeUInt16 uDivision = 0;
	SeUInt16 uThreshold = 0;
	SeDouble dAmplitude = 0.0;
	SeUInt8 nChannel = 1;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}else{
				uTime = strtoul(sTemp, SeNull, 10);;
			}
		}else if(iParamIndex == 1)
		{
			uDivision = strtoul(sTemp, SeNull, 10);;
		}else{
			uThreshold = strtoul(sTemp, SeNull, 10);;
		}
	}

	SeFpgaConfigTime(nChannel, uTime);
	SeFpgaConfigDivision(nChannel, uDivision);
	SeFpgaConfigThreshold(nChannel, uThreshold);
	
	//SeFpgaStartAD(nChannel);
	SeFpgaStartMeasure(nChannel);
	SeDelayMs(uTime);
	if(SeFpgaGetState(nChannel) == SE_RETURN_ERROR)
	{
		SeNormalPrint("amplitude measure error!");
		return SE_RETURN_ERROR;
	}

	if(SeFpgaGetAmplitudeMeasureResult(nChannel, &dAmplitude) == SE_RETURN_ERROR)
	{
		SeNormalPrint("amplitude measure get result error!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("ACK(%f Hz)", dAmplitude);

	return SE_RETURN_OK;
}

SeInt8 SeFpgaCodecOpenPdmHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: codec open pdm({<frequency>},{<volt>})\n \
			frequency:(100-100000)Hz   default 1000 Hz \n \
            volt: (1-3300) mV  default 3300 mV";

	SeInt8 iRet;
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeUInt8 nChannel = 1;
	SeInt16 iParamIndex = 0;
	SeUInt32 nFrequency = 1000;  // default 1000Hz
	SeUInt16 nVolt = 3300;  // default 3300mV

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}else{
				nFrequency = strtoul(sTemp, SeNull, 10);;
			}
		} else if(iParamIndex == 1) {
			nVolt =strtoul(sTemp, SeNull, 10);;
		}
	}

	// 复位通道
	iRet = SeFpgaDisableChannel(nChannel);
	iRet &= SeFpgaEnableChannel(nChannel);

	iRet &= SeFpgaPdmConfigFrequency(nChannel, nFrequency);
	iRet &= SeFpgaPdmConfigVolt(nChannel, nVolt);
	iRet &= SeFpgaPdmSelectSignalSource(nChannel, 0x01);
	iRet &= SeFpgaPdmStart(nChannel);

	return iRet;
}

SeInt8 SeFpgaCodecClosePdmHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: codec close pdm()";

	SeInt8 iRet;
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeUInt8 nChannel = 1;
	SeInt16 iParamIndex = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}
		}
	}

	iRet = SeFpgaPdmSelectSignalSource(nChannel, 0x01);
	iRet &= SeFpgaPdmStop(nChannel);

	return iRet;
}

SeInt8 SeFpgaThdnMeasureHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "thdn measure ({<-option>},{<chanel>},{<band>},{<timeout>},{<harmcount>}) \n \
			option:(d,f,n,t,s)\n \
			default=fn, not show some result which use for debug,and show THDN&Freq&VPP,\n \
                d: show the some result use for debug.\n \
                f: measure signal base frequency and vpp.\n \
				n: measure signal THD+N value.\n \
			    t: measure signal THD value.\n \
			    s: measure signal SINAD value. \n \
			channel:(left,right,dual)\n \
            band: (24-95977) Hz   band width frequency..\n \
            timeout: (1-10000) ms  measure timeout time,default 3000 ms.\n \
			harmcount: (2-11) default is 5";

	SeInt8 iRet;
	SeUInt16 iLength;
	SeChar sTemp[SE_BUFFER_SIZE_64] = {0};
	SeInt16 iParamIndex = 0;
	SeInt16 iBaseParamIndex = 0;
	SeUInt8 nChannel = 1;
	SeUInt16 nTimeout = 5000;		// default 3000ms
	SeUInt16 nBand = 20000;
	SeUInt32 nHarmCount = 5;
	SeUInt8 nSoundChannel = 0x03; // 0x00=none, 0x01=left, 0x02=right, 0x03=daul
	SeUInt8 nOption = 0x12;  // 命令选项字 0x01=debug, 0x02=thdn, 0x04=thd, 0x08=sinad, 0x10=freq

	SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, 0);
	memset(sTemp, 0, sizeof(sTemp));
	strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);

	if(SeParameterIsAskForHelp(pItem))
	{
		SeNormalPrint("%s", sUsage);
		return SE_RETURN_OK;
	} else {
		if(sTemp[0] == '-')
		{
			SeUInt8 iOptionIndex = 1;
			while(sTemp[iOptionIndex] != '\0')
			{
				switch (sTemp[iOptionIndex])
				{
					case 'd':
						nOption |= FPGA_PARAMETER_OPTION_DEBUG;
						break;
					case 't':
						nOption |= FPGA_PARAMETER_OPTION_THD;
						break;
					case 'n':
						nOption |= FPGA_PARAMETER_OPTION_THDN;
						break;
					case 's':
						nOption |= FPGA_PARAMETER_OPTION_SINAD;
						break;
					case 'f':
						nOption |= FPGA_PARAMETER_OPTION_FREQ;
						break;
					default:
						break;
				 }

				iOptionIndex++;
			}
		}
	}

	for(iParamIndex = iBaseParamIndex; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == iBaseParamIndex) {
			nSoundChannel = SeFpgaThdnGetSoundChannel(sTemp);
		} else if(iParamIndex == 1 + iBaseParamIndex) {
			nBand = strtoul(sTemp, SeNull, 10);;
		} else if(iParamIndex == 2 + iBaseParamIndex) {
			nTimeout = strtoul(sTemp, SeNull, 10);;
		} else if(iParamIndex == 3 + iBaseParamIndex) {
			nHarmCount = strtoul(sTemp, SeNull, 10);;
		}
	}

	iRet = SeFpgaDisableChannel(nChannel);
	iRet &= SeFpgaEnableChannel(nChannel);

	iRet &= SeFpgaThdConfigBandwidth(nChannel, nBand);
	iRet &= SeFpgaStartMeasure(nChannel);

	if(SeFpgaMeasureIsTimeout(nChannel, nTimeout))
	{
		return SE_RETURN_ERROR;
	}

	iLength = sprintf(sTemp,"%s", "ACK(");
	if(nOption & FPGA_PARAMETER_OPTION_THDN == FPGA_PARAMETER_OPTION_THDN)
	{
		SeDouble dThdn;
		iRet &= SeFpgaThdGetThdnResult(nChannel, &dThdn);
		iLength = sprintf(sTemp + iLength, "%f dB,", dThdn);
	}

	if(nOption & FPGA_PARAMETER_OPTION_THD == FPGA_PARAMETER_OPTION_THD)
	{
		SeDouble dThd;
		iRet &= SeFpgaThdGetThdResult(nChannel, nHarmCount, &dThd);
		iLength = sprintf(sTemp + iLength, "%f dB,", dThd);
	}

	if(nOption & FPGA_PARAMETER_OPTION_SINAD == FPGA_PARAMETER_OPTION_SINAD)
	{
		SeDouble dSinad;
		iRet &= SeFpgaThdGetSinadResult(nChannel, &dSinad);
		iLength = sprintf(sTemp + iLength, "%f dB,", dSinad);
	}

	if(nOption & FPGA_PARAMETER_OPTION_FREQ == FPGA_PARAMETER_OPTION_FREQ)
	{
		SeDouble dFreq, dMvolt;
		iRet &= SeFpgaThdGetFreqResult(nChannel, &dFreq, &dMvolt);
		iLength = sprintf(sTemp + iLength, "%f Hz,%f mV,", dFreq, dMvolt);
	}

	sprintf(sTemp + iLength, "%s", ")");
	SeNormalPrint(sTemp);
	return iRet;
}

SeInt8 SeFpgaUartConfigHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: fpga uart config(<channel>,<baud>,<bits>,<stop>,<parity>,{<timestamp>},{<echo>})\n \
			channel:(uart,spam)\n \
            baud:() dec\n \
            bits: (8)\n \
            stop: (1,1.5,2)\n \
            parity:(none,odd,even)\n \
            timestamp:(ON,OFF) default: ON\n \
                ON: add timestamp front line start.\n \
                OFF: not addr timestamp. \n \
            echo:(ON,OFF)  default OFF.\n \
                ON: echo what you send.\n \
                OFF: not echo.";

	SeInt8 iRet;
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeUInt8 nChannel = 1;
	SeInt16 iParamIndex = 0;
	SeUInt32 nBaudrate = 115200; //波特率 default 115200
	SeUInt8 nBits = 8,nStop = 1, nParity = 3;
	SeBool blAddTimestamp = SeTrue;
	SeBool blEcho = SeFalse;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}else{
				if(strcmp(sTemp, "uart") == 0)
					nChannel = 0x12;
				 else if(strcmp(sTemp, "spam") == 0)
					nChannel = 0x13;
			}
		} else if(iParamIndex == 1) {
			nBaudrate = strtoul(sTemp, SeNull, 10);;
		} else if(iParamIndex == 2) {
			nBits = strtoul(sTemp, SeNull, 10);;
		} else if(iParamIndex == 3) {
			nStop = strtoul(sTemp, SeNull, 10);;
		} else if(iParamIndex == 4) {
			nParity = strtoul(sTemp, SeNull, 10);;
		} else if(iParamIndex == 5) {
			if(strcmp(sTemp, "ON") == 0)
				blAddTimestamp = SeTrue;
			else if(strcmp(sTemp, "OFF") == 0)
				blAddTimestamp = SeFalse;
		} else if(iParamIndex == 6) {
			if(strcmp(sTemp, "ON") == 0)
				blEcho = SeTrue;
			else if(strcmp(sTemp, "OFF") == 0)
				blEcho = SeFalse;
		}
	}

	iRet = SeFpgaDisableChannel(nChannel);
	iRet &= SeFpgaEnableChannel(nChannel);
	iRet = SeFpgaUartConfig(nChannel, nBaudrate, nBits, nStop, nParity, blAddTimestamp, blEcho);
	if(iRet == SE_RETURN_OK)
		SeNormalPrint("ACK(DONE)");

	return iRet;
}

SeInt8 SeFpgaPulseWidthMeasureHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: pulse width measure(<time>,<division>,<threshold>)";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeUInt8 nChannel = 1;
	SeInt16 iParamIndex = 0;
	SeUInt16 uTime = 1;
	SeUInt16 uDivision = 0;
	SeUInt16 uThreshold = 0;
	SeFpgaPulseWidth tPulseWidth;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}else{
				uTime = strtoul(sTemp, SeNull, 10);;
			}
		}else if(iParamIndex == 1)
		{
			uDivision = strtoul(sTemp, SeNull, 10);;
		}else if(iParamIndex == 2){
			uThreshold =strtoul(sTemp, SeNull, 10);
		}
	}

	SeFpgaConfigTime(nChannel, uTime);
	SeFpgaConfigDivision(nChannel, uDivision);
	SeFpgaConfigThreshold(nChannel, uThreshold);
	
	//SeFpgaStartAD(nChannel);
	SeFpgaStartMeasure(nChannel);
	SeDelayMs(uTime);
	if(SeFpgaGetState(nChannel) == SE_RETURN_ERROR)
	{
		SeNormalPrint("pulse width measure error!");
		return SE_RETURN_ERROR;
	}

	if(SeFpgaGetPulseWidthMeasureResult(nChannel, &tPulseWidth) == SE_RETURN_ERROR)
	{
		SeNormalPrint("pulse width measure get result error!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("ACK(duty_ratio: %0.2f %%, pulse_low_count: %u, low_min: %u us, low_max: %u us, low_average: %f us, low_variance: %f us,pulse_high_count: %u, high_min: %u us, high_max: %u us, high_average: %f us, high_variance: %f us)",
		tPulseWidth.dDutyRatio,
		tPulseWidth.nLowCycleCount, tPulseWidth.nLowMin, tPulseWidth.nLowMax,
		tPulseWidth.dLowAverage, tPulseWidth.dLowVariance,
		tPulseWidth.nHighCycleCount, tPulseWidth.nHighMin, tPulseWidth.nHighMax,
		tPulseWidth.dHighAverage, tPulseWidth.dHighVariance);

	return SE_RETURN_OK;
}


SeInt8 SeSystemVersionInforGetHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "useage: version({number}) | number=(0~255) 1=mcu software,2=hardware,3=fpga bom,4=fpga,5=mcu software compile time,other=all information";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeInt16 iParamIndex = 0;
	SeInt16 iVersionIndex = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}
			else
			{
				iVersionIndex = strtoul(sTemp, SeNull, 10);;
			}
		}
		else
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}

	if(iVersionIndex < 0 || iVersionIndex > 255)
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}

	switch(iVersionIndex)
	{
		case 1:
			sprintf(sResult, "ACK(mcu software version: %s)", SeSoftwareVersionGet());
			break;
		case 2:
			sprintf(sResult, "ACK(hardware version: %s)", SeHardwareVersionGet());
			break;
		case 3:
			sprintf(sResult, "ACK(fpga bom version: %s)", SeFpgaBomVersionGet());
			break;
		case 4:
			sprintf(sResult, "ACK(fpga version: %s)", SeFpgaVersionGet());
			break;
		case 5:
			sprintf(sResult, "ACK(compile time: %s)", SeSoftwareCompileTimeGet());
			break;
		default:
			sprintf(sResult, "ACK(mcu software version: %s\r\n,hardware version: %s\r\n,fpga bom version: %s\r\n,fpga version: %s\r\n,compile time: %s\r\n,Author: %s)",
					SeSoftwareVersionGet(), SeHardwareVersionGet(), SeFpgaBomVersionGet(), SeFpgaVersionGet(), SeSoftwareCompileTimeGet(), SeAuthorNameGet());
	}

	return SE_RETURN_OK;
}

SeInt8 SeSystemConfigureSetHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "useage: system set({-type},{content}) | type=(mac,ip,netmask,gateway,sn,date,time),content=(mac<01:23:45:67:89:ab>,ip<192.168.1.100>,netmask<255.255.255.0>,"
			"gateway<192.168.1.1>,sn<1234567890seeing>,date<2016/01/10>,time<14:30:25>)";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeChar sType[SE_BUFFER_SIZE_16] = {0};
	SeChar sContent[SE_BUFFER_SIZE_32] = {0};

	SeInt16 iParamIndex = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}
			else
			{
				memcpy(sType, sTemp, strlen(sTemp));
			}
		}
		else if(iParamIndex == 1)
		{
			memcpy(sContent, sTemp, strlen(sTemp));
		}
	}

	if(strcmp("-mac", sType) == 0)
	{
		if(SeSystemMacSet(sContent) == SE_RETURN_ERROR)
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}
	else if(strcmp("-ip", sType) == 0)
	{
		if(SeSystemIpSet(sContent) == SE_RETURN_ERROR)
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}
	else if(strcmp("-netmask", sType) == 0)
	{
		if(SeSystemNetmaskSet(sContent) == SE_RETURN_ERROR)
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}
	else if(strcmp("-gateway", sType) == 0)
	{
		if(SeSystemGatewaySet(sContent) == SE_RETURN_ERROR)
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}
	else if(strcmp("-sn", sType) == 0)
	{
		if(SeSystemSerialNumberSet(sContent) == SE_RETURN_ERROR)
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}
	else if(strcmp("-date", sType) == 0)
	{
		if(SeSystemDateSet(sContent) == SE_RETURN_ERROR)
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}
	else if(strcmp("-time", sType) == 0)
	{
		if(SeSystemTimeSet(sContent) == SE_RETURN_ERROR)
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}
	else
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeSystemConfigureGetHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "useage: system read({-type}) | type=(mac,ip,netmask,gateway,sn,date,time)";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeChar sType[SE_BUFFER_SIZE_16] = {0};
	SeChar sContent[SE_BUFFER_SIZE_32] = {0};
	SeInt16 iParamIndex = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}
			else
			{
				memcpy(sType, sTemp, strlen(sTemp));
			}
		}
		else
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}

	if(strcmp("-mac", sType) == 0)
	{
		sprintf(sResult, "mac: %02x:%02x:%02x:%02x:%02x:%02x", tNetworkInfor.bMac[0],  tNetworkInfor.bMac[1],  tNetworkInfor.bMac[2],  tNetworkInfor.bMac[3], tNetworkInfor.bMac[4],  tNetworkInfor.bMac[5]);
	}
	else if(strcmp("-ip", sType) == 0)
	{
		sprintf(sResult, "ip: %u.%u.%u.%u", tNetworkInfor.bIp[0],  tNetworkInfor.bIp[1],  tNetworkInfor.bIp[2],  tNetworkInfor.bIp[3]);
	}
	else if(strcmp("-netmask", sType) == 0)
	{
		sprintf(sResult, "netmask: %u.%u.%u.%u", tNetworkInfor.bNetmask[0],  tNetworkInfor.bNetmask[1],  tNetworkInfor.bNetmask[2],  tNetworkInfor.bNetmask[3]);
	}
	else if(strcmp("-gateway", sType) == 0)
	{
		sprintf(sResult, "gateway: %u.%u.%u.%u", tNetworkInfor.bGateway[0],  tNetworkInfor.bGateway[1],  tNetworkInfor.bGateway[2],  tNetworkInfor.bGateway[3]);
	}
	else if(strcmp("-sn", sType) == 0)
	{
		sprintf(sContent, "sn: %s", SeSystemSerialNumberGet());
		sprintf(sResult, "%s", sContent);
	}
	else if(strcmp("-date", sType) == 0)
	{
		sprintf(sResult, "date: %04u/%02u/%02u", tSystemTime.iYear, tSystemTime.iMonth, tSystemTime.iDay);
	}
	else if(strcmp("-time", sType) == 0)
	{
		sprintf(sResult, "time: %02u:%02u:%02u", tSystemTime.iHour, tSystemTime.iMinute, tSystemTime.iSecond);
	}
	else
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeE2promReadHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "useage: eeprom read({pcb name},{type},{address},{count}) | pcb name=(arm,dmm,lna,datalogger,audio),"
			"type=(at24c01,at24c02,at24c04,at24c08,at24c16,at24c32,at24c64,at24c128,at24c256,at24c512)";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeChar aBytes[SE_BUFFER_SIZE_32] = {0};
	SeChar sReadDatas[SE_BUFFER_SIZE_64] = {0};
	SeChar sPcbName[SE_BUFFER_SIZE_16]={0};
	SeChar sE2promType[SE_BUFFER_SIZE_16]={0};
	SeUInt16 hStartAddress;
	SeInt16 nByteToRead;
	SeInt16 iParamIndex = 0;
	SeInt16 iBytesIndex = 0;
	SeInt8 iE2promIndex = 0;
	SeInt16 iLength = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}
			else
			{
				strncpy(sPcbName, sTemp,strlen(sTemp));
			}
		}
		else if(iParamIndex == 1)
		{
			strncpy(sE2promType, sTemp,strlen(sTemp));
		}
		else if(iParamIndex == 2)
		{
			hStartAddress = strtoul(sTemp, SeNull, 16);
		}
		else if(iParamIndex == 3)
		{
			nByteToRead = strtoul(sTemp, SeNull, 10);
		}
	}
	if(nByteToRead <= 0)
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}
#if 0
	if(SeE2promIndexGet(sPcbName, sE2promType, iE2promIndex) == SE_RETURN_ERROR || iE2promIndex < 0)
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}
	if(SeE2promRead(iE2promIndex, hStartAddress, aBytes, nByteToRead) == SE_RETURN_ERROR)
	{
		SeNormalPrint("system e2prom read error!");
		return SE_RETURN_ERROR;
	}
	else
	{
		for(iBytesIndex = 0; iBytesIndex < nByteToRead; iBytesIndex++)
		{
			iLength += sprintf(sReadDatas+iLength, "0x%x, ", aBytes[iBytesIndex]);
		}
		sReadDatas[iLength-1] = '\0';
		SeNormalPrint("ACK(%s)", sReadDatas);
	}
#endif
	return SE_RETURN_OK;
}

SeInt8 SeE2promWriteHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "useage: eeprom write({pcb name},{type},{address},{count},{data...}) | pcb name=(arm,dmm,lna,datalogger,audio),"
			"type=(at24c01,at24c02,at24c04,at24c08,at24c16,at24c32,at24c64,at24c128,at24c256,at24c512)";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeChar aBytes[SE_BUFFER_SIZE_32] = {0};
	SeChar sReadDatas[SE_BUFFER_SIZE_64] = {0};
	SeChar sPcbName[SE_BUFFER_SIZE_16]={0};
	SeChar sE2promType[SE_BUFFER_SIZE_16]={0};
	SeUInt16 hStartAddress;
	SeInt16 nByteToWrite;
	SeInt16 iParamIndex = 0;
	SeInt16 iBytesIndex = 0;
	SeInt8 iE2promIndex = 0;
	SeInt16 iLength = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}
			else
			{
				strncpy(sPcbName, sTemp,strlen(sTemp));
			}
		}
		else if(iParamIndex == 1)
		{
			strncpy(sE2promType, sTemp,strlen(sTemp));
		}
		else if(iParamIndex == 2)
		{
			hStartAddress = strtoul(sTemp, SeNull, 16);
		}
		else if(iParamIndex == 3)
		{
			nByteToWrite = strtoul(sTemp, SeNull, 10);
		}
		else
		{
			aBytes[iBytesIndex++] = strtoul(sTemp, SeNull, 16);
		}
	}
	if(nByteToWrite != iBytesIndex)
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}
#if 0
	if(SeE2promIndexGet(sPcbName, sE2promType, iE2promIndex) == SE_RETURN_ERROR || iE2promIndex < 0)
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}
	if(SeE2promWrite(iE2promIndex, hStartAddress, aBytes, nByteToWrite) == SE_RETURN_ERROR)
	{
		SeNormalPrint("system e2prom write error!");
		return SE_RETURN_ERROR;
	}
	else
	{
		SeNormalPrint("ACK(DONE)");
	}
#endif
	return SE_RETURN_OK;
}

SeInt8 SeE2promReadStringHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "useage: eeprom read({pcb name},{type},{address},{count}) | pcb name=(arm,dmm,lna,datalogger,audio),"
			"type=(at24c01,at24c02,at24c04,at24c08,at24c16,at24c32,at24c64,at24c128,at24c256,at24c512)";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeChar aBytes[SE_BUFFER_SIZE_32] = {0};
	SeChar sPcbName[SE_BUFFER_SIZE_16]={0};
	SeChar sE2promType[SE_BUFFER_SIZE_16]={0};
	SeUInt16 hStartAddress;
	SeInt16 nByteToRead;
	SeInt16 iParamIndex = 0;
	SeInt8 iE2promIndex = 0;


	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}
			else
			{
				strncpy(sPcbName, sTemp,strlen(sTemp));
			}
		}
		else if(iParamIndex == 1)
		{
			strncpy(sE2promType, sTemp,strlen(sTemp));
		}
		else if(iParamIndex == 2)
		{
			hStartAddress = strtoul(sTemp, SeNull, 16);
		}
		else if(iParamIndex == 3)
		{
			nByteToRead = strtoul(sTemp, SeNull, 10);
		}
	}
	if(nByteToRead <= 0)
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}
#if 0
	if(SeE2promIndexGet(sPcbName, sE2promType, iE2promIndex) == SE_RETURN_ERROR || iE2promIndex < 0)
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}
	if(SeE2promRead(iE2promIndex, hStartAddress, aBytes, nByteToRead) == SE_RETURN_ERROR)
	{
		SeNormalPrint("system e2prom read error!");
		return SE_RETURN_ERROR;
	}
	else
	{
		SeNormalPrint("ACK(%s)", aBytes);
	}
#endif
	return SE_RETURN_OK;
}

SeInt8 SeE2promWriteStringHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "useage: eeprom write({pcb name},{type},{address},{string}) | pcb name=(arm,dmm,lna,datalogger,audio),"
			"type=(at24c01,at24c02,at24c04,at24c08,at24c16,at24c32,at24c64,at24c128,at24c256,at24c512)";
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeChar aBytes[SE_BUFFER_SIZE_32] = {0};
	SeChar sReadDatas[SE_BUFFER_SIZE_64] = {0};
	SeChar sPcbName[SE_BUFFER_SIZE_16]={0};
	SeChar sE2promType[SE_BUFFER_SIZE_16]={0};
	SeUInt16 hStartAddress;
	SeInt16 nByteToWrite;
	SeInt16 iParamIndex = 0;
	SeInt16 iBytesIndex = 0;
	SeInt8 iE2promIndex = 0;
	SeInt16 iLength = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}
			else
			{
				strncpy(sPcbName, sTemp,strlen(sTemp));
			}
		}
		else if(iParamIndex == 1)
		{
			strncpy(sE2promType, sTemp,strlen(sTemp));
		}
		else if(iParamIndex == 2)
		{
			hStartAddress = strtoul(sTemp, SeNull, 16);
		}
		else if(iParamIndex == 3)
		{
			memcpy(aBytes, sTemp+1, strlen(sTemp)-2);
		}
	}
#if 0
	if(SeE2promIndexGet(sPcbName, sE2promType, iE2promIndex) == SE_RETURN_ERROR || iE2promIndex < 0)
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}
	if(SeE2promWrite(iE2promIndex, hStartAddress, aBytes, nByteToWrite) == SE_RETURN_ERROR)
	{
		SeNormalPrint("system e2prom write error!");
		return SE_RETURN_ERROR;
	}
	else
	{
		SeNormalPrint("ACK(DONE)");
	}
#endif
	return SE_RETURN_OK;
}


SeInt8 SeSystemEchoHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "useage: echo({string})";

	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeInt16 iParamIndex = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}
			else
			{
				SeNormalPrint("ACK(%s)", sTemp);
			}
		}
		else
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}
	return SE_RETURN_OK;
}

#define SE_UPDATE_REQUEST 0
#define SE_UPDATE_CONFIRM 1
#define SE_UPDATE_MCU 2
#define SE_UPDATE_FPGA 3
SeInt8 SeUpdateHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: update(<module>,<fun-number>))"
			"module:(MCU,FPGA,SFPGA)"
			    "MCU: update MCU firmware."
			    "FPGA: update FPGA firmware."
			"fun-number:(0,1) "
			    "0: request to do some thing. "
			    "1: confirm and do some thing,if it request pass.)";

	SeInt8 iRet = SE_RETURN_OK;
	SeChar sModule[SE_BUFFER_SIZE_32] = {0};
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeInt16 iParamIndex = 0;
	SeUInt8 nFunctionNumber;
	static nUpdateFlag = 0;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}

			strncpy(sModule, sTemp, pItem->nParameterLength);
		}
		else if(iParamIndex == 1)
		{
			nFunctionNumber = strtoul(sTemp, SeNull, 10);
		}
		else
		{
			SeNormalPrint("please check the command parameters!");
			return SE_RETURN_ERROR;
		}
	}

	if(strncmp(sModule, "MCU", 3) == 0)
	{
		if (nFunctionNumber == SE_UPDATE_REQUEST)
		{
			nUpdateFlag = SE_UPDATE_MCU;
		}
		else if(nFunctionNumber == SE_UPDATE_CONFIRM && nUpdateFlag == SE_UPDATE_MCU)
		{
			iRet = SeUpdateMcuXmodem();
		}
	} else if(strncmp(sModule, "FPGA", 4) == 0)
	{
		if (nFunctionNumber == SE_UPDATE_REQUEST)
		{
			nUpdateFlag = SE_UPDATE_FPGA;
		}
		else if(nFunctionNumber == SE_UPDATE_CONFIRM && nUpdateFlag == SE_UPDATE_FPGA)
		{
			iRet = SeUpdateFpgaXmodem();
		}

	}
	else
	{
		SeNormalPrint("please check the command parameters!");
		return SE_RETURN_ERROR;
	}

	return iRet;
}

SeInt8 SeAd9265DataAcquisitionHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: datalogger({ip},{port},{period})";

	SeInt16 iParamIndex = 0;
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeNetworkAcquisitionDescription tNetAcqDesc;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
			}else{
				sscanf(sTemp, "%d.%d.%d.%d", &(tNetAcqDesc.tServerIp.aIpAddress[0]), &(tNetAcqDesc.tServerIp.aIpAddress[1]), &(tNetAcqDesc.tServerIp.aIpAddress[2]), &(tNetAcqDesc.tServerIp.aIpAddress[3]));
			}
		}else if(iParamIndex == 1){
			tNetAcqDesc.tServerPort = strtoul(sTemp, SeNull, 10);
		}else{
			tNetAcqDesc.tNetworkAcquisitionInformation.nSampleTime = strtoul(sTemp, SeNull, 10);
		}
	}

	tNetAcqDesc.tNetworkAcquisitionOperation.fpSeNetworkAcquisitionPrepare = SeNetworkAcquisitionAd9265Prepare;
	tNetAcqDesc.tNetworkAcquisitionOperation.fpSeNetworkAcquisitionGetDataSize = SeNetworkAcquisitionAd9265GetDataSize;
	tNetAcqDesc.tNetworkAcquisitionOperation.fpSeNetworkAcquisitionReadData = SeNetworkAcquisitionAd9265ReadData;
	tNetAcqDesc.tNetworkAcquisitionOperation.fpSeNetworkAcquisitionFinish = SeNetworkAcquisitionAd9265Finish;

	if(SeNetworkAcquisitionSessionCreate(tNetAcqDesc) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeTftpUpdateHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: tftp_update({ip},{port},{filename},{module})";

	SeInt16 iParamIndex = 0;
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeChar sFileName[SE_BUFFER_SIZE_32] = {0};
	SeTftpClientDescription tTftpDescription;
	SeTftpClientGetFileOperation tTftpOperation;
	SeUpdateApplicationType tUpdateType;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(SeParameterIsAskForHelp(pItem))
			{
				SeNormalPrint("%s", sUsage);
				return SE_RETURN_OK;
			}else{
				sscanf(sTemp, "%d.%d.%d.%d", &(tTftpDescription.tServerIp.aIpAddress[0]), &(tTftpDescription.tServerIp.aIpAddress[1]), &(tTftpDescription.tServerIp.aIpAddress[2]), &(tTftpDescription.tServerIp.aIpAddress[3]));
			}
		}else if(iParamIndex == 1)
		{
			tTftpDescription.tServerPort = strtoul(sTemp, SeNull, 10);
		}else if(iParamIndex == 2)
		{
			strncpy(sFileName, sTemp, sizeof(sTemp));
		}else if(iParamIndex == 3)
		{
			if(strncmp(sTemp, "FPGA", 4) == 0)
			{
				tUpdateType = SeUpdateApplicationTypeFpga;
			}else if(strncmp(sTemp, "MCU", 3) == 0)
			{
				tUpdateType = SeUpdateApplicationTypeMcu;
			}else{
				tUpdateType = SeUpdateApplicationTypeNone;
			}
		}
	}

	tTftpOperation = SeTftpUpdateOperationGet();

	if(SeTftpClientGetFile(tTftpDescription, sFileName, SeTftpFileTypeOctet, tTftpOperation, sTemp, tUpdateType) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

