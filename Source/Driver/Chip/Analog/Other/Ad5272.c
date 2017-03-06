/*
 * Ad5272.c
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#include "Ad5272.h"

static SeAd5272Description tAd5272Desc;

#define AD5272_RWA              100000
#define AD5272_CODE_MAX         1024
#define AD5272_ADDR_BASE        0x58
#define AD5272_A1A0             0
#define AD5272_ADDR             (AD5272_ADDR_BASE|(AD5272_A1A0<<1))
#define AD5272_RE_FLAG          1
#define AD5272_WR_FLAG          0
#define AD5272_NOP              0x0000
#define AD5272_WR_RDAC          0x0400
#define AD5272_RE_RDAC          0x0800
#define AD5272_STOR_RDAC        0x0c00
#define AD5272_SOFT_RST         0x1000
#define AD5272_RE_STOR          0x1400
#define AD5272_RE_STOR_ADD      0x1800
#define AD5272_WR_CTRLRG        0x1C00
#define AD5272_RDAC_STOR_EN     0x0003
#define AD5272_RE_CTRLRG        0x2000
#define AD5272_SOFT_OFF         0x2400

#define SeAd5272Start()                     SeI2cStart(tAd5272Desc.iAd5272I2cIndex)
#define SeAd5272Stop()                      SeI2cStop(tAd5272Desc.iAd5272I2cIndex)
#define SeAd5272Ack()                       SeI2cAck(tAd5272Desc.iAd5272I2cIndex)
#define SeAd5272Nack()                      SeI2cNack(tAd5272Desc.iAd5272I2cIndex)
#define SeAd5272ReadByte(pData, blAck)      SeI2cRead(tAd5272Desc.iAd5272I2cIndex, pData, blAck)
#define SeAd5272WriteByte(bData, blAck)     SeI2cWrite(tAd5272Desc.iAd5272I2cIndex, bData, blAck)

SeInt8 SeAd5272Init(SeAd5272Description tAd5272Description)
{
    if(tAd5272Description.iAd5272I2cIndex == 0 || tAd5272Description.bAd5272DeviceAddress == 0)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd5272WriteData(tAd5272Description.bAd5272DeviceAddress, AD5272_WR_CTRLRG|AD5272_RDAC_STOR_EN) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
}

SeInt8 SeAd5272WriteData(SeUInt8 hAddress, SeUInt16 iData)
{
	SeBool blGetAck = SeFalse;
	
    if(SeAd5272Start() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}	
	if((SeAd5272WriteByte(hAddress|AD5272_WR_FLAG, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
	{
		goto I2cCmdFail;
	}
	if((SeAd5272WriteByte(iData>>8, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
	{
		goto I2cCmdFail;
	}
    if((SeAd5272WriteByte(iData, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
	{
		goto I2cCmdFail;
	}    
	if(SeAd5272Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	} 
    return SE_RETURN_OK;
I2cCmdFail:
	if(SeAd5272Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR;
}

SeInt8 SeAd5272ReadData(SeUInt8 hAddress, SeUInt16 *pData)
{
    SeUInt8 bData = 0;
	SeBool blSendAck = SeFalse;
	SeBool blGetAck = SeFalse;
	
    if(pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		*pData = 0;
	}
	if(SeAd5272Start() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}	
	if((SeAd5272WriteByte(hAddress|AD5272_RE_FLAG, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
	{
		goto I2cCmdFail;
	}
    if(SeAd5272ReadByte(&bData, blSendAck) == SE_RETURN_ERROR)
	{
		goto I2cCmdFail;
	}
    if(SeAd5272Ack() == SE_RETURN_ERROR)
	{
		goto I2cCmdFail;
	}
    *pData = bData;
	
    if(SeAd5272ReadByte(&bData, blSendAck) == SE_RETURN_ERROR)
	{
		goto I2cCmdFail;
	}
    if(SeAd5272Nack() == SE_RETURN_ERROR)
	{
		goto I2cCmdFail;
	}
    if(SeAd5272Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}    
    *pData = ((*pData)<<8)|bData;    
    return SE_RETURN_OK;
I2cCmdFail:
	if(SeAd5272Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR;
}

SeInt8 SeAd5272SetResistance(SeUInt8 hAddress, SeFloat fResistance)
{
    SeUInt16 iData = 0;
    SeDouble fTemp = 0.0;
    
    fTemp = fResistance;
    iData = fTemp/AD5272_RWA*AD5272_CODE_MAX+0.5;
    if(iData > 1024)
	{
		return SE_RETURN_ERROR;
	}
    if(SeAd5272WriteData(hAddress, AD5272_WR_RDAC|iData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}  
  	
	return SE_RETURN_OK;
}

SeInt8 SeAd5272ReadRdacReg(SeUInt8 hAddress, SeUInt16 *pRdacData)
{
    SeUInt16 iData = 0;
    
	if(pRdacData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		*pRdacData = 0;
	}
    if(SeAd5272WriteData(hAddress, AD5272_RE_RDAC) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
		 
    if(SeAd5272ReadData(hAddress, &iData) == SE_RETURN_ERROR)
    {
		return SE_RETURN_ERROR;
	}
    *pRdacData = iData&0x3ff;

    return SE_RETURN_OK;
}

SeInt8 SeAd5272ReadResistance(SeUInt8 hAddress, SeFloat *pResistance)
{
    SeUInt16 iRdacData = 0;
	
	if(pResistance == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		*pResistance = 0.0;
	}
    if(SeAd5272ReadRdacReg(hAddress, &iRdacData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    *pResistance = 1.0 *iRdacData/AD5272_CODE_MAX*AD5272_RWA;
    
    return SE_RETURN_OK;
}

SeInt8 SeAd5272SetRdacStore(SeUInt8 hAddress)
{
    /* Save RDAC reg value to 50-TP */
	if(SeAd5272WriteData(hAddress, AD5272_STOR_RDAC) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
}

SeInt8 SeAd5272IsSetRdacStoreOK(SeUInt8 hAddress)
{
    SeUInt16 iData = 0;
    
    if(SeAd5272WriteData(hAddress, AD5272_RE_CTRLRG) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd5272WriteData(hAddress, AD5272_NOP) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd5272ReadRdacReg(hAddress, &iData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}	
    if((iData&0x0008) > 0)
	{
		return SE_RETURN_OK;
	}
    else
	{
		return SE_RETURN_ERROR;
	}    
}

