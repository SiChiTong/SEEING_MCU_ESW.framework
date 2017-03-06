/*
 * Fpga.c
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#include "Fpga.h"

static void SeFpgaPrintInformation(void)
{
	SeFpgaSystemFpgaVersionRegister tFpgaVersion;
	SeFpgaSystemBomVersionRegister tBomVersion;
	SeFpgaSystemPhyIdRegister tPhyId;
	SeFpgaSystemFpgaTypeRegister tFpgaType;
	SeFpgaSystemFpgaPageTypeRegister tPageType;

	SeNormalPrint("Querying FPGA informations...");

	SeUInt16 hAddress = (SE_FPGA_SYSTEM_PAGE_ADDRESS << 8)|SeFpgaSystemFpgaVersionRegisterAddress;
	SeFpgaRead8BitRegister(hAddress, &tFpgaVersion);

	hAddress = (SE_FPGA_SYSTEM_PAGE_ADDRESS << 8)|SeFpgaSystemBomVersionRegisterAddress;
	SeFpgaRead8BitRegister(hAddress, &tBomVersion);

	hAddress = (SE_FPGA_SYSTEM_PAGE_ADDRESS << 8)|SeFpgaSystemPhyIdRegisterAddress;
	SeFpgaRead8BitRegister(hAddress, &tPhyId);

	hAddress = (SE_FPGA_SYSTEM_PAGE_ADDRESS << 8)|SeFpgaSystemFpgaTypeRegisterAddress;
	SeFpgaRead8BitRegister(hAddress, &tFpgaType);

	SeNormalPrint("#FPGA Version:(%02Xh)", tFpgaVersion);
	SeNormalPrint("#FPGA BOM:(%02Xh)", tBomVersion.Byte);
	SeNormalPrint("#FPGA PHY ID:(%02Xh)", tPhyId.Byte);
	switch(tFpgaType)
	{
		case SeFpgaChipType_XC6SLX45_CSG324:
			SeNormalPrint("#FPGA Chip Type:(XILINX XC6SLX45-CSG324)");
			break;
		default:
			SeNormalPrint("#FPGA Chip Type:(<%02Xh>,Unknown)", tFpgaType);
			break;
	}

	SeUInt8 iPageIndex = 1;
	SeUInt8 nPages = SeFpgaSystemPage15TypeRegisterAddress - SeFpgaSystemPage1TypeRegisterAddress + 1;
	hAddress = (SE_FPGA_SYSTEM_PAGE_ADDRESS << 8)|SeFpgaSystemPage1TypeRegisterAddress;
	for(iPageIndex = 1; iPageIndex <= nPages; iPageIndex++)
	{
		SeChar aPageName[SE_BUFFER_SIZE_128] = {0};
		SeFpgaRead8BitRegister(hAddress, &tPageType);
		switch(tPageType)
		{
			case SeFpgaSystemPageFunctionMeasure:
				strcat(aPageName, "Frequency Measure");
				break;
			case SeFpgaSystemPageFunction192KHzDatalogger:
				strcat(aPageName, "Datalogger(Low Speed)");
				break;
			case SeFpgaSystemPageFunctionUart:
				strcat(aPageName, "UART");
				break;
			case SeFpgaSystemPageFunctionThdn:
				strcat(aPageName, "THD-N Measure");
				break;
			case SeFpgaSystemPageFunctionPdm:
				strcat(aPageName, "PDM");
				break;
			case SeFpgaSystemPageFunction125MHzDatalogger:
				strcat(aPageName, "Datalogger(Middle Speed)");
				break;
			case SeFpgaSystemPageFunctionAd9265Driver:
				strcat(aPageName, "AD9265");
				break;
			default:
				strcat(aPageName, "Unknown");
				break;
		}
		SeNormalPrint("#FPGA Page%d function:(<%02Xh>,%s)", iPageIndex, tPageType, aPageName);
		hAddress++;
	}
	SeNormalPrint("Done!");
}

SeInt8 SeFpgaRegisterInit(void)
{
	SeFpgaPrintInformation();
	return SE_RETURN_OK;
}

SeInt8 SeFpgaRegisterRead(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nLength)
{
	return SeFpgaReadDatas(hAddress, pData, nLength);
}

SeInt8 SeFpgaRegisterWrite(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nLength)
{
	return SeFpgaWriteDatas(hAddress, pData, nLength);
}

SeInt8 SeFpgaConfigTime(SeUInt8 nChannel, SeUInt16 nTime)
{
	SeUInt16 nAddr =  (nChannel << 8) | FPGA_MEASURE_TIME_REG;
	return SeFpgaWrite16BitRegister(nAddr, (SeFpgaSpiRegisterType16Bit)nTime);
}

SeInt8 SeFpgaConfigDivision(SeUInt8 nChannel, SeUInt16 nDivision)
{
	SeUInt16 nAddr =  (nChannel << 8) | FPGA_DIVISION_REG;
	return SeFpgaWrite16BitRegister(nAddr, (SeFpgaSpiRegisterType16Bit)nDivision);
}

SeInt8 SeFpgaConfigThreshold(SeUInt8 nChannel, SeUInt16 nThreshold)
{
	SeUInt16 nAddr =  (nChannel << 8) | FPGA_THRESHOLD_REG;
	return SeFpgaWrite16BitRegister(nAddr, (SeFpgaSpiRegisterType16Bit)nThreshold);
}

SeInt8 SeFpgaEnableChannel(SeUInt8 nChannel)
{
	SeUInt16 nAddr = (nChannel << 8) | FPGA_ENABLE_CHANNEL_REG;
	return SeFpgaWrite8BitRegister(nAddr, 1);
}

SeInt8 SeFpgaDisableChannel(SeUInt8 nChannel)
{
	SeUInt16 nAddr = (nChannel << 8) | FPGA_ENABLE_CHANNEL_REG;
	return SeFpgaWrite8BitRegister(nAddr, 0);
}

SeInt8 SeFpgaStartMeasure(SeUInt8 nChannel)
{
	SeUInt16 nAddr = (nChannel << 8) | FPGA_MEASURE_START_REG;
	return SeFpgaWrite8BitRegister(nAddr,0);
}

SeInt8 SeFpgaGetState(SeUInt8 nChannel)
{
    SeUInt8 nState = 0;
    SeUInt16 nAddr = (nChannel << 8) | FPGA_MEASURE_STATE_REG;;

    if(SeFpgaRead8BitRegister(nAddr, &nState) < 0)
        return SE_RETURN_ERROR;

    return (nState == 1) ? SE_RETURN_ERROR : SE_RETURN_OK;
}

SeBool SeFpgaMeasureIsTimeout(SeUInt8 nChannel, SeUInt32 nTimeout)
{
	SeFpgaSpiRegisterType8Bit nState;
	SeUInt16 nAddr = (nChannel << 8) | FPGA_MEASURE_STATE_REG;
	SeTimeValue tCurrentTime = SeGetCurrentTimeValue();
	SeUInt64 nTimeoutTime = tCurrentTime.iMicroSeconds + nTimeout;

	do {
		if(SeFpgaRead8BitRegister(nAddr, &nState) == SE_RETURN_OK)
		{
			if(nState == 1)
				return SeFalse;
		}

		SeDelayMs(10);
		tCurrentTime = SeGetCurrentTimeValue();
	}while(tCurrentTime.iMicroSeconds < nTimeoutTime);

	return SeTrue;
}

static SeUInt64 SeMeasureDataToUInt64(SeUInt8 *pData)
{
    SeInt8 iCount;
    SeUInt64 nTemp = 0;

    for(iCount = 7; iCount >= 0; iCount--)
    {
        nTemp = nTemp << 8;
        nTemp |= pData[iCount];
    }

    return nTemp;
}

static SeDouble SeFpgaFrequencyLsmAlgorithm(SeUInt16 nCycleCnt, SeUInt32 nClock, SeUInt16 nDivision,
		SeUInt8 *pXY, SeUInt8 *pY, SeUInt8 *pX, SeUInt8 *pXX)
{
    SeInt8 iRet;
    SeUInt8 nSize = 0;
    SeUInt64 nTemp;
    SeDouble frequency;
    SeUInt8 aTemp1[32] = {0};
    SeUInt8 aTemp2[32] = {0};
    SeUInt8 aTemp3[32] = {0};
    SeUInt8 iCount, nCycleCount;

    memset(aTemp1, 0, sizeof(aTemp1));
    memset(aTemp2, 0, sizeof(aTemp2));

    for(iCount = 0; iCount < 8;  iCount++)
    {
    	aTemp1[iCount] = pX[iCount];
        aTemp2[iCount] = pX[iCount];
    }

	// aTemp1 = lsm_x^2
    SeLargeNumberMul(aTemp1, sizeof(aTemp1), aTemp2, sizeof(aTemp2));

	// aTemp2 = lsm_cyc_cnt * lsm_xxn
    memset(aTemp2, 0, sizeof(aTemp2));
    nCycleCount = nCycleCnt;
    for(iCount = 0; iCount < 2; iCount++)
    {
    	aTemp2[iCount] = nCycleCount & 0xff;
        nCycleCount = nCycleCount >> 8;
    }
    SeLargeNumberMul(aTemp2, sizeof(aTemp2), pXX, 8);

    //aTemp1 = lsm_cyc_cnt * lsm_xxn-lsm_x^2
    SeLargeNumberSub(aTemp1, sizeof(aTemp1), aTemp2, sizeof(aTemp2));

    //aTemp3 = lsm_x * lsm_y
    memset(aTemp3, 0, sizeof(aTemp3));
	for(iCount = 0; iCount < 8; iCount++)
    {
		aTemp3[iCount] = pX[iCount];
    }
	SeLargeNumberMul(aTemp3, sizeof(aTemp3), pY, 8);

	//aTemp2 = lsm_cyc_cnt * lsm_xy
    memset(aTemp3, 0, sizeof(aTemp3));
    nCycleCount = nCycleCnt;
    for(iCount = 0; iCount < 2; iCount++)
    {
    	aTemp2[iCount] = nCycleCount & 0xff;
    	nCycleCount = nCycleCount >> 8;
    }
    SeLargeNumberMul(aTemp2, sizeof(aTemp2), pXY, 8);

	//aTemp2 = lsm_cyc_cnt * lsm_xy -l sm_x * lsm_y
    SeLargeNumberSub(aTemp2, sizeof(aTemp2), aTemp3, sizeof(aTemp3));

    //aTemp3 = clk_freq * (lsm_cyc_cnt * lsm_xxn - lsm_x^2)
	memset(aTemp2, 0, sizeof(aTemp2));
    nTemp = nClock;
    nTemp *= 1000;   //KHz->Hz
    nTemp *= 0x5f5e100;      //10000 0000, 保留8位小数
    for(iCount = 0; iCount < 8; iCount++)
    {
    	aTemp3[iCount] = nTemp & 0xff;
    	nTemp = nTemp >> 8;
    }
    SeLargeNumberMul(aTemp3, sizeof(aTemp3), aTemp1, sizeof(aTemp1));

    //m = clk_freq * (lsm_cyc_cnt * lsm_xxn-lsm_x^2) / (lsm_cyc_cnt * lsm_xy - lsm_x * lsm_y)
    memset(aTemp1, 0, sizeof(aTemp1));
    nSize = sizeof(aTemp1);
    iRet = SeLargeNumberDiv(aTemp3, sizeof(aTemp3), aTemp2, sizeof(aTemp2), aTemp1, &nSize);

    if(iRet >= 0)
    {
    	nTemp = 0;
        for(iCount = 7; iCount >= 0; iCount--)
        {
        	nTemp = nTemp << 8;
        	nTemp |= aTemp1[iCount];
        }

        frequency = (double)nTemp / 100000000.0;
        frequency *= nDivision;
    }  else {
    	frequency = 0.0;
    }

    return frequency;
}

SeInt8 SeFpgaGetFrequencyMeasureResult(SeUInt8 nChannel, SeFpgaFrequency *pFrequency)
{
    SeUInt16 nLsmCycleCnt;
    SeUInt32 nClockFrequency;
    SeUInt16 nAddr, nPage, nDivision;
    SeFpgaSpiRegisterType64Bit lsmXX, lsmX, lsmY, lsmXY;

    nPage = nChannel << 8;

    nAddr = nPage | FPGA_CLOCK_FREQUENCY_REG;
    if(SeFpgaRead24BitRegister(nAddr, &nClockFrequency) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    nAddr = nPage | FPGA_FREQ_LSM_CYCLE_CNT_REG;
    if(SeFpgaRead16BitRegister(nAddr, &nLsmCycleCnt) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    nAddr = nPage | FPGA_FREQ_LSM_XY_REG;
    if(SeFpgaReadDatas(nAddr, lsmXY.aData, 8) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    nAddr = nPage | FPGA_FREQ_LSM_Y_REG;
    if(SeFpgaReadDatas(nAddr, lsmY.aData, 8) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    nAddr = nPage | FPGA_FREQ_LSM_X_REG;
    if(SeFpgaReadDatas(nAddr, lsmX.aData, 8) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    nAddr = nPage | FPGA_FREQ_LSM_XX_N_REG;
    if(SeFpgaReadDatas(nAddr, lsmXX.aData, 8) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    nAddr = nPage | FPGA_DIVISION_REG;
    if(SeFpgaRead16BitRegister(nAddr, &nDivision) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    nDivision = nDivision > 0 ? nDivision : 1;
    pFrequency->dFrequency = SeFpgaFrequencyLsmAlgorithm(nLsmCycleCnt, nClockFrequency, nDivision,
    		lsmXY.aData, lsmY.aData, lsmX.aData, lsmXX.aData);

    pFrequency->nClock = nClockFrequency;
    pFrequency->nDivision = nDivision;
    pFrequency->nX = lsmX.hData;
    pFrequency->nY = lsmY.hData;
    pFrequency->nXY = lsmXY.hData;
    pFrequency->nXX = lsmXX.hData;

	return SE_RETURN_OK;
}


SeInt8 SeFpgaGetAmplitudeMeasureResult(SeUInt8 nChannel, double *pAmplitude)
{
    int iCount;
    SeUInt8 nSize;
    SeUInt64 nTemp;
    SeUInt16 nPage, nAddr, nCheckVPP;
    SeUInt8 aVppSum[8], aVppCycleCount[8];
    SeUInt8 aTemp1[32] = {0};
    SeUInt8 aTemp2[32] = {0};

    nPage = nChannel << 8;

    // 读取测量量程
    nAddr = nPage | FPGA_AMPLITUDE_CHECK_VPP_REG;
    if(SeFpgaRead16BitRegister(nAddr, &nCheckVPP) < 0)
    	return SE_RETURN_ERROR;

    // 读取峰峰值累加值
    nAddr = nPage | FPGA_AMPLITUDE_VPP_SUM_REG;
    if(SeFpgaReadDatas(nAddr, aVppSum, 8) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    // 读取峰峰值测量周期数
    nAddr = nPage | FPGA_AMPLITUDE_VPP_CYCLE_COUNT_REG;
    if(SeFpgaReadDatas(nAddr, aVppCycleCount, 8) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    // 计算 amplitude = (check_vpp * vpp_sum)/(vpp_cyc_cnt * 65536)
    // k = check_vpp * vpp_sum
    nTemp = nCheckVPP;
    nTemp *= 0x5f5e100;      // *100 000 000, 保留8位小数
    for(iCount = 0; iCount < 8; iCount++)
    {
        aTemp1[iCount] = nTemp & 0xff;
        nTemp = nTemp >> 8;
    }
    SeLargeNumberMul(aTemp1, sizeof(aTemp1), aVppSum, 8);

    nSize = sizeof(aTemp2);
    if(SeLargeNumberDiv(aTemp1, sizeof(aTemp1), aVppCycleCount, 8, aTemp2, &nSize) < 0)
    {
    	*pAmplitude = 0.0;
    }    else    {
        nTemp = SeMeasureDataToUInt64(aTemp2);
        // TODO luanma
        *pAmplitude = (nTemp / (65536 * 100000000.0));

    }

	return SE_RETURN_OK;
}

SeInt8 SeFpgaGetPulseWidthMeasureResult(SeUInt8 nChannel, SeFpgaPulseWidth *pPulseWidth)
{
    SeUInt16 nPage, nAddr;
    SeUInt64 nSum, nSquareSum;

    nPage = nChannel << 8;

    // 读取低脉宽周期数
    nAddr = nPage | FPGA_PULSE_WIDTH_LOW_CYCLE_CNT_REG;
    if(SeFpgaRead24BitRegister(nAddr, &pPulseWidth->nLowCycleCount) < 0)
    	return SE_RETURN_ERROR;

    // 读取低脉宽最大值
    nAddr = nPage | FPGA_PULSE_WIDTH_LOW_MAX_REG;
    if(SeFpgaRead24BitRegister(nAddr, &pPulseWidth->nLowMax) < 0)
    	return SE_RETURN_ERROR;

    // 读取低脉宽最小值
    nAddr = nPage | FPGA_PULSE_WIDTH_LOW_MIN_REG;
    if(SeFpgaRead24BitRegister(nAddr, &pPulseWidth->nLowMin) < 0)
    	return SE_RETURN_ERROR;

    // 读取低脉宽累加值
    nAddr = nPage | FPGA_PULSE_WIDTH_LOW_SUM_REG;
    if(SeFpgaRead48BitRegister(nAddr, &nSum) < 0)
    	return SE_RETURN_ERROR;

    // 读取低脉宽平方和
    nAddr = nPage | FPGA_PULSE_WIDTH_LOW_SQUARE_SUM_REG;
    if(SeFpgaRead48BitRegister(nAddr, &nSquareSum) < 0)
    	return SE_RETURN_ERROR;

    // TODO luanma

    // 低脉宽均值计算 pulse_low_average = pulse_low_sum / (pulse_low_cnt + 1)
    pPulseWidth->dLowAverage = nSum / (pPulseWidth->nLowCycleCount + 1);

    // 低脉宽方差计算 pulse_low_variance = pulse_low_square_sum / (pulse_low_cnt + 1) - pulse_low_average
    pPulseWidth->dLowVariance = nSquareSum / (pPulseWidth->nLowCycleCount + 1) -
    		pPulseWidth->dLowAverage * pPulseWidth->dLowAverage;

    // 读取高脉宽周期数
    nAddr = nPage | FPGA_PULSE_WIDTH_HIGH_CYCLE_CNT_REG;
    if(SeFpgaRead24BitRegister(nAddr, &pPulseWidth->nHighCycleCount) < 0)
    	return SE_RETURN_ERROR;

    // 读取高脉宽最大值
    nAddr = nPage | FPGA_PULSE_WIDTH_HIGH_MAX_REG;
    if(SeFpgaRead24BitRegister(nAddr, &pPulseWidth->nHighMax) < 0)
    	return SE_RETURN_ERROR;

    // 读取高脉宽最小值
    nAddr = nPage | FPGA_PULSE_WIDTH_HIGH_MIN_REG;
    if(SeFpgaRead24BitRegister(nAddr, &pPulseWidth->nHighMin) < 0)
    	return SE_RETURN_ERROR;

    // 读取高脉宽累加值
    nAddr = nPage | FPGA_PULSE_WIDTH_HIGH_SUM_REG;
    if(SeFpgaRead48BitRegister(nAddr, &nSum) < 0)
    	return SE_RETURN_ERROR;

    // 读取高脉宽平方和
    nAddr = nPage | FPGA_PULSE_WIDTH_HIGH_SQUARE_SUM_REG;
    if(SeFpgaRead48BitRegister(nAddr, &nSquareSum) < 0)
    	return SE_RETURN_ERROR;

    // TODO luanma

    // 高脉宽均值计算 pulse_high_average = pulse_high_sum / (pulse_high_cnt + 1)
    pPulseWidth->dHighAverage = nSum / (pPulseWidth->nHighCycleCount + 1);

    // 高脉宽方差计算 pulse_high_variance = pulse_high_square_sum / (pulse_high_cnt + 1) - pulse_high_average
    pPulseWidth->dHighVariance = nSquareSum / (pPulseWidth->nHighCycleCount + 1) -
    		pPulseWidth->dHighAverage * pPulseWidth->dHighAverage;

    // 计算占空比
    pPulseWidth->dDutyRatio = (pPulseWidth->dHighAverage*100.0) / (pPulseWidth->dHighAverage + pPulseWidth->dLowAverage);

	return SE_RETURN_OK;
}

SeInt8 SeFpgaPdmConfigFrequency(SeUInt8 nChannel, SeUInt32 nFrequency)
{
	SeDouble dFreq = 0x100000000;
	SeFpgaSpiRegisterType32Bit nClock;
	SeUInt16 nPage =  (nChannel << 8);
	SeUInt16 nAddr = nPage | FPGA_CLOCK_FREQUENCY_REG;

	if(SeFpgaRead32BitRegister(nAddr, &nClock) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	if(nClock.hData == 0)
		return SE_RETURN_ERROR;

	// TODO luanma
	dFreq *= nFrequency;
	dFreq /= 1000;
	dFreq /= (SeDouble)nClock.hData;
	nFrequency = (SeUInt32)dFreq;

	nAddr = nPage | FPGA_PDM_FREQ_CTRL_REG;
	return SeFpgaWrite32BitRegister(nAddr, (SeFpgaSpiRegisterType32Bit)nFrequency);
}

SeInt8 SeFpgaPdmConfigVolt(SeUInt8 nChannel, SeUInt16 nVolt)
{
	SeDouble dVolt;
	SeFpgaSpiRegisterType16Bit nVpp;
	SeUInt16 nPage = nChannel << 8;
	SeUInt16 nAddr = nPage | FPGA_VPP_DATA_REG;

	if(SeFpgaRead16BitRegister(nAddr,&nVpp) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	dVolt = nVolt * 65535 / nVpp.hData;
	nVolt = (SeUInt16)dVolt;
	nAddr = nPage | FPGA_PDM_VOLT_CTRL_REG;
	return SeFpgaWrite16BitRegister(nAddr, (SeFpgaSpiRegisterType16Bit)nVolt);
}

SeInt8 SeFpgaPdmSelectSignalSource(SeUInt8 nChannel, SeUInt8 nSource)
{
	SeUInt16 nAddr = (nChannel << 8) | FPGA_PDM_SOURCE_SELECT_REG;
	return SeFpgaWrite8BitRegister(nAddr, nSource);
}

SeInt8 SeFpgaPdmStart(SeUInt8 nChannel)
{
	SeUInt16 nAddr = (nChannel << 8) | FPGA_PDM_SIGNAL_CTRL_REG;
	return SeFpgaWrite8BitRegister(nAddr, 1);
}

SeInt8 SeFpgaPdmStop(SeUInt8 nChannel)
{
	SeUInt16 nAddr = (nChannel << 8) | FPGA_PDM_SIGNAL_CTRL_REG;
	return SeFpgaWrite8BitRegister(nAddr, 0);
}

SeInt8 SeFpgaUartConfig(SeUInt8 nChannel, SeUInt32 nBaudrate,  SeUInt8 nBits,
		SeUInt8 nStop, SeUInt8 nParity, SeBool blAddTimestamp, SeBool blEcho)
{
    SeUInt16 nAddr, nPage;
    SeUInt8 nConfig;
    SeFpgaSpiRegisterType32Bit nClock;
    SeUInt64 nTemp;

    nPage = nChannel << 8;

    nAddr = nPage | FPGA_CLOCK_FREQUENCY_REG;
    if(SeFpgaRead32BitRegister(nAddr,&nClock) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;


    //reg = (baudrate /clk)*2^32
    //TODO luanma
    nTemp = 0x100000000;
    nTemp *= nBaudrate;
    nTemp /= nClock.hData;
    nTemp /= 1000;
    nBaudrate = nTemp;


    nAddr =nPage | FPGA_UART_BAUD_RATE_REG;
    if(SeFpgaWrite32BitRegister(nAddr, (SeFpgaSpiRegisterType32Bit)nBaudrate) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    nConfig = 0;

    // 数据位长度
    if(5 == nBits)
        nConfig |= 4 << 4;
    else if(6 == nBits)
    	nConfig |= 5<<4;
    else if(7 == nBits)
    	nConfig |= 6<<4;
    else
    	nConfig |= 7<<4;

    // 停止位
    if(2 == nStop)
    	nConfig |= 2<<1;     //1.5
    else if(3 == nStop)
    	nConfig |= 3<<1;     //2
    else
    	nConfig |= 1<<1;     //1

    // 校验位
    if(0 == nParity)
    	nConfig |= 0;        //odd
    else if(1 == nParity)
    	nConfig |= 1;        //even
    else
    	nConfig |= 5;        //none

    nAddr = nPage | FPGA_UART_PARAMETER_REG;
    if(SeFpgaWrite8BitRegister(nAddr, nConfig) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;


    nConfig = 0;
    //时间戳
    if(blAddTimestamp)
        nConfig |= 0x02;
    else
        nConfig &= (~0x02);

    //回显
    if(blEcho)
        nConfig |= 0x01;
    else
    	nConfig &= (~0x01);

    nAddr = nPage | FPGA_UART_FUNCTION_REG;
    if(SeFpgaWrite8BitRegister(nAddr, nConfig) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    // 载入参数
    nAddr = nPage | FPGA_UART_CONFIG_LOAD_REG;
    if(SeFpgaWrite8BitRegister(nAddr, 0x01) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    return SE_RETURN_OK;
}


