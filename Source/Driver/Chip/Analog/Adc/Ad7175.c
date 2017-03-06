/*
 * Ad7175_Adc.c
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */

#include "Ad7175.h"

static SeAd7175Description tAd7175Desc;

#define AD7175_CHANNEL_AMOUNT		 4
#define READ_REGISTER_FLAG					0x40
#define WRITE_REGISTER_FLAG                 0xbf
#define AD7175_ID                           0x00CD  
#define AD7175_MODE_REGISTER                0x01                  
#define AD7175_DATA_REGISTER                0x04                  
#define AD7175_ID_REGISTER                  0x07        
#define AD7175_CHANNEL_DISABLE              0x0001
#define AD7175_ADMODE_BASE                  0x800C     
#define AD7175_MEASURE_TIMEOUT_COUNT        200000
#define AD7175_RESET_TIMEOUT_COUNT          70
#define SeAd7175WaitMeasure()               SeSystemDelayUs(1) 
#define SeAd7175ReadDataOut(bData)          SeGpioGetBit(tAd7175Desc.iAd7175DoutPinIndex, bData)
#define SeAd7175Stop()                      SeGpioSetBit(tAd7175Desc.iAd7175CsPinIndex, SE_BIT_HIGH)
#define SeAd7175WriteByte(bData)            SeSpiWrite(tAd7175Desc.iAd7175SpiIndex, bData)
#define SeAd7175ReadByte(pData)             SeSpiRead(tAd7175Desc.iAd7175SpiIndex, pData)

SeUInt8 Ad7175ChannelRegAddress[AD7175_CHANNEL_AMOUNT] ={0x10,0x11,0x12,0x13};
SeUInt16 Ad7175ChannelRegValue[AD7175_CHANNEL_AMOUNT] ={0x8001,0x9043,0x0001,0x0001};
SeUInt8  Ad7175OffsetRegAddress[AD7175_CHANNEL_AMOUNT] = {0x30,0x31,0x32,0x33};
SeUInt8 Ad7175GainRegAddress[AD7175_CHANNEL_AMOUNT] = {0x38,0x39,0x3a,0x3b};
SeUInt8  Ad7175FilterRegAddress[AD7175_CHANNEL_AMOUNT] ={0x28,0x29,0x2a,0x2b};
SeUInt16 Ad7175FilterRegValue[AD7175_CHANNEL_AMOUNT] ={0x0514,0x0514,0x0514,0x0514};
SeUInt8  Ad7175SetupRegAddress[AD7175_CHANNEL_AMOUNT] ={0x20,0x21,0x22,0x23};
SeUInt16 Ad7175SetupRegValue[AD7175_CHANNEL_AMOUNT] ={0x1f00,0x1f00,0x1f00,0x1f00};

SeInt8 SeAd7175Init(SeAd7175Description tAd7175Description)
{ 	
	if(tAd7175Description.iAd7175SpiIndex == 0 || tAd7175Description.iAd7175DoutPinIndex == 0 || tAd7175Description.iAd7175CsPinIndex == 0)
	{
		return SE_RETURN_ERROR;
	}

    return SE_RETURN_OK;
}

static SeInt8 SeAd7175SendBytes(SeUInt8 *pData, SeUInt8 nLength)
{
    SeInt32 iBytesIndex = 0;
    
	if(pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	
    for(iBytesIndex = nLength-1; iBytesIndex >= 0; iBytesIndex--)
    {
        if(SeAd7175WriteByte(pData[iBytesIndex]) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;

		}
    }
	
	return SE_RETURN_OK;
}

static SeInt8 SeAd7175ReceiveBytes(SeUInt8 *pData, SeUInt8 nLength)
{
    SeInt32 iBytesIndex = 0;
	
	if(pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	
    for(iBytesIndex = nLength-1; iBytesIndex >= 0; iBytesIndex--)
    {
        if(SeAd7175ReadByte(&pData[iBytesIndex]) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
    }
	
	return SE_RETURN_OK;
}

SeInt8 SeAd7175Start(SeAd7175ChannelNumber tChannel, SeAd7175ModeType tMode)
{   
    SeInt8 iChannelIndex = 0;
	
	/* Disable used channel */
	SeUInt16 hChannelValue[4] = {AD7175_CHANNEL_DISABLE, AD7175_CHANNEL_DISABLE, AD7175_CHANNEL_DISABLE, AD7175_CHANNEL_DISABLE};
    /* Select channel */
	hChannelValue[tChannel] = Ad7175ChannelRegValue[tChannel];
	
	for(iChannelIndex = 0; iChannelIndex < AD7175_CHANNEL_AMOUNT; iChannelIndex++)
	{
		if(SeAd7175WriteRegistor(Ad7175ChannelRegAddress[iChannelIndex], 2 , hChannelValue[iChannelIndex]) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
	}    
    if(SeAd7175ModeSet(tMode) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
}

SeInt8 SeAd7175WriteRegistor(SeUInt8 hAddress, SeUInt8 nSize, SeUInt32 iValue)
{
    SeUInt8 bBuffer[3] ={0};
    SeUInt8 iBytesIndex = 0;
    
    for( iBytesIndex = 0; iBytesIndex < 3; iBytesIndex++)
    {
        bBuffer[iBytesIndex] = iValue & 0xff;
        iValue = iValue >> 8;
    }
    
    if(SeAd7175SendBytes(&hAddress, 1) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd7175SendBytes(bBuffer, nSize>3?3:nSize) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
}

SeInt8 SeAd7175ReadRegistor(SeUInt8 hAddress, SeUInt8 nSize, SeUInt32 *pData)
{
    SeUInt8 bBuffer[3] = {0};
    SeInt8 iBytesIndex = 0;
    
	if(pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		*pData = 0;
	}
	
    if(SeAd7175SendBytes(&hAddress, 1) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd7175ReceiveBytes(bBuffer, nSize>3?3:nSize) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
   
    for( iBytesIndex = 2; iBytesIndex>=0; iBytesIndex--)
    {
        *pData = *pData<<8;
        *pData |= bBuffer[iBytesIndex];
    }
    
    return SE_RETURN_OK;
}

SeInt8 SeAd7175ReadConvertData(SeAd7175ChannelNumber tChannel, SeAd7175ModeType tMode, SeUInt32 *pData)
{
	SeUInt8 hAddress = 0;
    SeUInt32 iValue = 0;
    
    if(pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		*pData = 0;
	}
	/* Read finish convert value */
	hAddress = AD7175_DATA_REGISTER|READ_REGISTER_FLAG;
	
    if(SeAd7175SendBytes(&hAddress, 1) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd7175ReceiveBytes((SeUInt8 *)(&iValue), 3) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	*pData = iValue;
	
	if((tMode == IoCalMode) || (tMode == SoCalMode))
	{
		/* Read calibration value */
        hAddress = Ad7175OffsetRegAddress[tChannel]|READ_REGISTER_FLAG;
		iValue = 0;
        if(SeAd7175SendBytes(&hAddress, 1) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
        if(SeAd7175ReceiveBytes((SeUInt8 *)(&iValue), 3) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
		*pData = iValue;	
	}
	else if(tMode == SgCalMode)
	{
       /* Read system inside calibration gain value */
		hAddress = Ad7175GainRegAddress[tChannel]|READ_REGISTER_FLAG;
        iValue = 0;
		if(SeAd7175SendBytes(&hAddress, 1) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
        if(SeAd7175ReceiveBytes((SeUInt8 *)(&iValue), 3) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
		*pData = iValue;	
	}
    
	return SE_RETURN_OK;
}

SeInt8 SeAd7175GetSampleData(SeAd7175ChannelNumber tChannel, SeAd7175ModeType tMode, SeUInt32 *pData)
{
    SeUInt32 nCount = 0;
	SeUInt8 iRet = 0;
	
    if(pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	/* AD7175 start measure */
    if(SeAd7175Start(tChannel, tMode) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    /* Query the measure state */
    do{
        SeAd7175WaitMeasure();
        nCount++;
        if(nCount >= AD7175_MEASURE_TIMEOUT_COUNT)
        {
            if(SeAd7175Stop() == SE_RETURN_ERROR)
			{
				return SE_RETURN_ERROR;
			}
			return SE_RETURN_ERROR;
        }
		iRet = SeAd7175IsMeasureOk();
    }while(SE_RETURN_OK != iRet);
   
    if(SeAd7175ReadConvertData(tChannel, tMode, pData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	   
	return SE_RETURN_OK;
}

SeInt8 SeAd7175ModeSet(SeAd7175ModeType tMode)
{
    SeUInt8 hAddress = 0;
    SeUInt16 iValue = 0;
    
    /* Config mode */
	hAddress = AD7175_MODE_REGISTER;
	iValue = AD7175_ADMODE_BASE|tMode;
    
    if(SeAd7175SendBytes(&hAddress, 1) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    if(SeAd7175SendBytes((SeUInt8 *)(&iValue), 2) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
}

SeInt8 SeAd7175IsMeasureOk(void)
{
	SeUInt8 bDate = 0;
	if(SeAd7175ReadDataOut(&bDate) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		if(bDate == 0)
		{
			return SE_RETURN_OK;
		}
		else
		{
			return SE_RETURN_ERROR;
		}
	}
}
