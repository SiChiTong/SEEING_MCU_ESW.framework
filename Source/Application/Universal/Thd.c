/*
 * Thd.c
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#include "Thd.h"

SeUInt8 SeFpgaThdnGetSoundChannel(SeChar *sChannel)
{
	SeUInt8 nSoundChannel;
	if(strcmp(sChannel,"left") == 0)
		nSoundChannel = 0x01;
	else if(strcmp(sChannel, "right") == 0)
		nSoundChannel = 0x02;
	else if(strcmp(sChannel, "dual") == 0)
		nSoundChannel = 0x03;
	else
		nSoundChannel = 0x00;

	return nSoundChannel;
}

SeInt8 SeFpgaThdConfigBandwidth(SeUInt8 nChannel, SeUInt16 nBand)
{
	SeUInt16 nAddr, nPage;
	SeFpgaSpiRegisterType24Bit nClock;
	SeUInt64 nTemp;

	nPage = nChannel << 8;
	nAddr = nPage | FPGA_CLOCK_FREQUENCY_REG;
	if(SeFpgaRead24BitRegister(nAddr, &nClock) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	//TODO luanma
	nTemp = nBand;
	nTemp *= 8192;
	nTemp /= nClock.hData;   //--KHz
	nTemp /= 1000;  //--Hz
	nBand = nTemp;

	nAddr = nPage | FPGA_THDN_BANDWIDTH_REG;
	if(SeFpgaWrite16BitRegister(nAddr, (SeFpgaSpiRegisterType16Bit)nBand) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	return SE_RETURN_OK;
}

SeInt8 SeFpgaThdGetThdnResult(SeUInt8 nChannel, SeDouble *pThdn)
{
	SeUInt16 nAddr;
	SeUInt8 iCount, nSizeQ;
	SeUInt16 nPage = nChannel << 8;
	SeUInt64 nTemp, nTemp1 = 10000000000000000;
	SeUInt8 aTempX[8], aTempY[8], aTempQ[8];
	SeFpgaSpiRegisterType64Bit nBasePower, nBandPower;

	nAddr = nPage |= FPGA_THDN_BASE_POWER_REG;
	if(SeFpgaRead64BitRegister(nAddr, &nBasePower) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	nAddr = nPage |= FPGA_THDN_BAND_POWER_REG;
	if(SeFpgaRead64BitRegister(nAddr, &nBandPower) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	if(nBasePower.hData == 0)
	{
		*pThdn = 99.999999;
		return SE_RETURN_OK;
	}

	nTemp = nBandPower.hData - nBasePower.hData;
	memset(aTempX, 0, sizeof(aTempX));
	memset(aTempY, 0, sizeof(aTempY));

	// x=x*1000000
	for(iCount = 0; iCount < 8; iCount++)
	{
		aTempX[iCount] = nTemp & 0xff;
		aTempY[iCount] = nTemp1 & 0xff;
		nTemp = nTemp >> 8;
		nTemp1 = nTemp1 >> 8;
	}

    if(SeLargeNumberMul(aTempX, sizeof(aTempX), aTempY, sizeof(aTempY)) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;


    //x = (BAND_POWER-BASE_POWER)/BASE_POWER
    memset(aTempY, 0, sizeof(aTempY));
    for(iCount = 0; iCount < 8; iCount++)
    {
        aTempY[iCount] = nBasePower.hData & 0xff;
        nBasePower.hData = nBasePower.hData >> 8;
    }

    memset(aTempQ, 0, sizeof(aTempQ));
    nSizeQ= sizeof(aTempQ);
    if(SeLargeNumberDiv(aTempX, sizeof(aTempX), aTempY, sizeof(aTempY), aTempQ, &nSizeQ) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    // 转换为浮点
    nTemp = 0;
    for(iCount = 7; iCount >= 0; iCount--)
    {
    	nTemp = nTemp << 8;
    	nTemp |= aTempQ[iCount];
    }

    //TODO luanma

    *pThdn = nTemp / 10000000000000000;
    *pThdn = log10(*pThdn);
    *pThdn *= 10;


    return SE_RETURN_OK;
}

SeInt8 SeFpgaThdGetThdResult(SeUInt8 nChannel, SeUInt32 nHarmCount, SeDouble *pThd)
{
	SeInt8 iCount;
    SeUInt16 nAddr, nPage;
    double dFreq,dMvolt,fTemp;

    SeFpgaSpiRegisterType24Bit nClock;
    SeFpgaSpiRegisterType64Bit nBasePower;
    SeFpgaSpiRegisterType16Bit nFftLength;
    SeFpgaSpiRegisterType48Bit nData;

    SeUInt16 nIndexAddr, nIndex, nDataAddr;
    SeInt16 nIndexK1= 0, nIndexK2 = 0;
    SeUInt64 nHiPower;

	nPage = nChannel << 8;

    if(SeFpgaThdGetFreqResult(nChannel, &dFreq, &dMvolt) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    if(0 == dFreq)
    {
        *pThd = 999.999999;
        return SE_RETURN_OK;
    }

	nAddr = nPage | FPGA_CLOCK_FREQUENCY_REG;
	if(SeFpgaRead24BitRegister(nAddr, &nClock) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

    nAddr = nPage | FPGA_THDN_BASE_POWER_REG;
    if(SeFpgaRead64BitRegister(nAddr, &nBasePower) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

    //FFT length
	nAddr = nPage | FPGA_THDN_FFT_LENGTH_REG;
	if(SeFpgaRead16BitRegister(nAddr, &nFftLength) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	// 计算K1, K2的位置, 11次谐波
    for(iCount = 1,fTemp = 0; iCount <= nHarmCount; iCount++)
        fTemp += dFreq * (iCount + 1);

    // TODO luanma

    fTemp *= nFftLength.hData;
    fTemp /= nClock.hData * 1000;
    nIndexK1 = fTemp - 1;
    nIndexK2 = nIndexK1 + 1;


    // 谐波包络位置
    nIndexK1 -= 4;
    nIndexK2 += 4;

    if(nIndexK1 < 0)
    {
        *pThd = 999.999999;
        return SE_RETURN_OK;
    }

    // 谐波包络能量
    //data addr
	nDataAddr = nPage | FPGA_THDN_FFT_DATA_REG;
    nIndexAddr = nPage | FPGA_THDN_FFT_INDEX_REG;
    for(nIndex = nIndexK1, nHiPower = 0; ((nIndex <= nIndexK2) && (nIndex <= nFftLength.hData)); nIndex++)
    {
        if(SeFpgaWrite16BitRegister(nIndexAddr, (SeFpgaSpiRegisterType16Bit)nIndex) == SE_RETURN_ERROR)
        	return SE_RETURN_ERROR;

        if(SeFpgaRead48BitRegister(nDataAddr,&nData) == SE_RETURN_ERROR)
        	return SE_RETURN_ERROR;

        nHiPower += nData.hData;
    }

    // TODO luanma

    nHiPower = nHiPower >> 3;
    fTemp = nHiPower / nBasePower.hData;
    *pThd = 10*log10(fTemp);


    return SE_RETURN_OK;
}

SeInt8 SeFpgaThdGetSinadResult(SeUInt8 nChannel, SeDouble *pSinad)
{
    SeDouble fTemp;
    SeUInt16 nAddr, nPage;
    SeUInt16 nIndexAddr, nIndex, nDataAddr;
    int nIndexTemp1, nIndexTemp2;
    SeUInt64 nDistPower;

    SeFpgaSpiRegisterType64Bit nBasePower;
    SeFpgaSpiRegisterType16Bit nFftLength;
    SeFpgaSpiRegisterType16Bit nDistIndexA;  // 最大失真位置A
    //SeFpgaSpiRegisterType48Bit nDistDataA;   // 最大失真数据A
    SeFpgaSpiRegisterType16Bit nDistIndexB;  // 最大失真位置B
    //SeFpgaSpiRegisterType48Bit nDistDataB;   // 最大失真数据B
    SeFpgaSpiRegisterType48Bit nData;

	nPage = nChannel << 8;
    nAddr = nPage | FPGA_THDN_BASE_POWER_REG;
    if(SeFpgaRead64BitRegister(nAddr, &nBasePower) == SE_RETURN_ERROR)
    	return SE_RETURN_ERROR;

	nAddr = nPage | FPGA_THDN_FFT_LENGTH_REG;
	if(SeFpgaRead16BitRegister(nAddr, &nFftLength) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	nAddr = nPage | FPGA_THDN_DIST_INDEX_A_REG;
	if(SeFpgaRead16BitRegister(nAddr, &nDistIndexA) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	//nAddr = nPage | FPGA_THDN_DIST_DATA_A_REG;
	//if(SeFpgaRead48BitRegister(nAddr, &nDistDataA) == SE_RETURN_ERROR)
	//	return SE_RETURN_ERROR;

	nAddr = nPage | FPGA_THDN_DIST_INDEX_B_REG;
	if(SeFpgaRead16BitRegister(nAddr, &nDistIndexB) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	//nAddr = nPage | FPGA_THDN_DIST_DATA_B_REG;
	//if(SeFpgaRead48BitRegister(nAddr, &nDistDataB) == SE_RETURN_ERROR)
	//	return SE_RETURN_ERROR;

    if(nDistIndexA.hData < nDistIndexB.hData)
    {
        nIndexTemp1 = nDistIndexA.hData;
        nIndexTemp2 = nDistIndexB.hData;
    }
    else
    {
        nIndexTemp1 = nDistIndexB.hData;
        nIndexTemp2 = nDistIndexA.hData;
    }

    nIndexTemp1 -= 4;
    nIndexTemp2 += 4;

    if(nIndexTemp1 < 0)
    {
        *pSinad = 999.999999;
        return SE_RETURN_OK;
    }

    // 失真包络能量
	nDataAddr = nPage | FPGA_THDN_FFT_DATA_REG;
    nIndexAddr = nPage | FPGA_THDN_FFT_INDEX_REG;
    for(nIndex = nIndexTemp1, nDistPower=0; ((nIndex <= nIndexTemp2) && (nIndex <= nFftLength.hData)); nIndex++)
    {
        if(SeFpgaWrite16BitRegister(nIndexAddr, (SeFpgaSpiRegisterType16Bit)nIndex) == SE_RETURN_ERROR)
        	return SE_RETURN_ERROR;

        if(SeFpgaRead48BitRegister(nDataAddr, &nData) == SE_RETURN_ERROR)
        	return SE_RETURN_ERROR;

        nDistPower += nData.hData;
    }
    //TODO luanma

    nDistPower = nDistPower >> 3;
    fTemp = nDistPower;
    fTemp /= nBasePower.hData;
    *pSinad= 10*log10(fTemp);

    return SE_RETURN_OK;
}

SeInt8 SeFpgaThdGetFreqResult(SeUInt8 nChannel, SeDouble *pFreq, SeDouble *pMvolt)
{
	SeUInt16 nAddr, nPage;
	SeUInt16 nTempK1;
    SeUInt64 nTempY1, nTempY2;
    SeDouble fTempB ,fTempA, fTemp;

	SeFpgaSpiRegisterType16Bit nFftLength;
	SeFpgaSpiRegisterType24Bit nClock;
	SeFpgaSpiRegisterType16Bit nVppRange;
	SeFpgaSpiRegisterType16Bit  nDiv;
	SeFpgaSpiRegisterType16Bit nFftIndexA;  // 基波位置A
	SeFpgaSpiRegisterType48Bit nFftDataA;   // 基波数据A
	SeFpgaSpiRegisterType16Bit nFftIndexB;  // 基波位置B
	SeFpgaSpiRegisterType48Bit nFftDataB;   // 基波数据B

	nPage = nChannel << 8;
	nAddr = nPage | FPGA_CLOCK_FREQUENCY_REG;
	if(SeFpgaRead24BitRegister(nAddr, &nClock) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

    nAddr = nPage | FPGA_THDN_VPP_RANGE_REG;
	if(SeFpgaRead16BitRegister(nAddr, &nVppRange) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	//FFT length
	nAddr = nPage | FPGA_THDN_FFT_LENGTH_REG;
	if(SeFpgaRead16BitRegister(nAddr, &nFftLength) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

    nAddr = nPage | FPGA_THDN_DIV_DATA_REG;
	if(SeFpgaRead16BitRegister(nAddr, &nDiv) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	nAddr = nPage | FPGA_THDN_BASE_INDEX_A_REG;
	if(SeFpgaRead16BitRegister(nAddr, &nFftIndexA) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	nAddr = nPage | FPGA_THDN_BASE_DATA_A_REG;
	if(SeFpgaRead48BitRegister(nAddr, &nFftDataA) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	nAddr = nPage | FPGA_THDN_BASE_INDEX_B_REG;
	if(SeFpgaRead16BitRegister(nAddr, &nFftIndexB) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	nAddr = nPage | FPGA_THDN_BASE_DATA_B_REG;
	if(SeFpgaRead48BitRegister(nAddr, &nFftDataB) == SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

    if(nFftIndexA.hData >= nFftIndexB.hData)
    {
        nTempK1 = nFftIndexB.hData;
        nTempY1 = nFftDataB.hData;
        nTempY2 = nFftDataA.hData;
    }    else    {
    	nTempK1 = nFftIndexA.hData + 1;
    	nTempY1 = nFftDataA.hData;
    	nTempY2 = nFftDataB.hData;
    }

    nTempY1 = sqrt(nTempY1);
    nTempY2 = sqrt(nTempY2);
    if(((0 == nTempY2) && (0 == nTempY1)) || (0 == nFftLength.hData))
    {
            *pFreq = 0;
            *pMvolt = 0;
            return SE_RETURN_OK;
    }

    // TODO luanma

    fTempB = nTempY2;
    fTempB -= nTempY1;
    fTempB /= (nTempY2 + nTempY1);
    fTempA =  2.95494514 * fTempB + 0.17671943 * pow(fTempB, 3) + 0.09230694 * pow(fTempB, 5);

    *pFreq = (0.5 + fTempA + nTempK1);
    *pFreq *= nClock.hData;  //KHz
    *pFreq *= 1000; //->Hz
    fTemp = nFftLength.hData * (nDiv.hData + 1);
    *pFreq /= fTemp;

    *pMvolt = (nTempY1 + nTempY2);
    *pMvolt *= (3.20976143 + 0.9187393 * pow(fTempA, 2) + 0.14734229 * pow(fTempA, 4));
    *pMvolt /= nFftLength.hData;

    *pMvolt *= nVppRange.hData;
    *pMvolt /= 1<<10;

    return SE_RETURN_OK;
}



