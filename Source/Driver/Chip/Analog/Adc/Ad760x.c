/*
 * Ad760x.c
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */

#include "Ad760x.h"

#ifndef CONFIG_USING_AD7606
#define ADC7606_USE     0
#else
#define ADC7606_USE		1
#endif

static SeAd760xDescription tAd760xDesc;
SeAd760xModeLogger tAd760xModeLogger;

#define AD760X_CONVERT_TIMEOUT_COUNT     800
#define SeAd760xConvertDelay()     SeSystemDelayUs(4) 
#define SeAd760xCheckDelay()       SeSystemDelayUs(1) 
#define SeAd760xClkHighDelay()     SeSystemDelayUs(1) 
#define SeAd760xClkLowDelay()      SeSystemDelayUs(1) 
#define SeAd760xResetDelay()       SeSystemDelayUs(100) 

#define SeAd760xReadDataOut(bData) SeGpioGetBit(tAd760xDesc.iAd760xDoutPinIndex, bData)
#define SeAd760xIsConvertOK(bData) SeGpioGetBit(tAd760xDesc.iAd760xBusyPinIndex, bData)

#define SeAd760xResetHigh()        SeGpioSetBit(tAd760xDesc.iAd760xResetPinIndex, SE_BIT_HIGH)
#define SeAd760xResetLow()         SeGpioSetBit(tAd760xDesc.iAd760xResetPinIndex, SE_BIT_LOW)

#define SeAd760xOs0High()          SeGpioSetBit(tAd760xDesc.iAd760xOs0PinIndex, SE_BIT_HIGH)
#define SeAd760xOs0Low()           SeGpioSetBit(tAd760xDesc.iAd760xOs0PinIndex, SE_BIT_LOW)
#define SeAd760xOs1High()          SeGpioSetBit(tAd760xDesc.iAd760xOs1PinIndex, SE_BIT_HIGH)
#define SeAd760xOs1Low()           SeGpioSetBit(tAd760xDesc.iAd760xOs1PinIndex, SE_BIT_LOW)
#define SeAd760xOs2High()          SeGpioSetBit(tAd760xDesc.iAd760xOs2PinIndex, SE_BIT_HIGH)
#define SeAd760xOs2Low()           SeGpioSetBit(tAd760xDesc.iAd760xOs2PinIndex, SE_BIT_LOW)

#define SeAd760xRangeHigh()        SeGpioSetBit(tAd760xDesc.iAd760xRangePinIndex, SE_BIT_HIGH)
#define SeAd760xRangeLow()         SeGpioSetBit(tAd760xDesc.iAd760xRangePinIndex, SE_BIT_LOW)

#define SeAd760xConvertHigh()      SeGpioSetBit(tAd760xDesc.iAd760xConvertPinIndex, SE_BIT_HIGH)
#define SeAd760xConvertLow()       SeGpioSetBit(tAd760xDesc.iAd760xConvertPinIndex, SE_BIT_LOW)

#define SeAd760xClkHigh()          SeGpioSetBit(tAd760xDesc.iAd760xSclkPinIndex, SE_BIT_HIGH)
#define SeAd760xClkLow()           SeGpioSetBit(tAd760xDesc.iAd760xSclkPinIndex, SE_BIT_LOW)

#define SeAd760xCsHigh()           SeGpioSetBit(tAd760xDesc.iAd760xCsPinIndex, SE_BIT_HIGH)
#define SeAd760xCsLow()            SeGpioSetBit(tAd760xDesc.iAd760xCsPinIndex, SE_BIT_LOW)

SeInt8 SeAd760xReset(void)
{
    if(SeAd760xResetHigh() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    SeAd760xResetDelay();
    if(SeAd760xResetLow() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
}

SeInt8 SeAd760xModeSet(SeAd760xSampleMode tOs)
{
	if((tOs&0x01) == 0x01)
	{
	 	if(SeAd760xOs0High() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
	}
	else
	{
		if(SeAd760xOs0Low() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}	
	}
	
	if((tOs&0x02) == 0x02)
	{
	 	if(SeAd760xOs1High() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
	}
	else
	{
		if(SeAd760xOs1Low() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}	
	}
	
	if((tOs&0x04) == 0x04)
	{
	 	if(SeAd760xOs2High() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
	}
	else
	{
		if(SeAd760xOs2Low() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}	
    }
    tAd760xModeLogger.iOs = tOs;  
	
	return SE_RETURN_OK;
}

SeInt8 SeAd760xRangeSet(SeAd760xRange tRange)
{	
    /* Config range = 1,+-10V,or range = 0,+-5V	*/
    if(Ad760xRange1 == tRange)
    {
        if(SeAd760xRangeHigh() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
        tAd760xModeLogger.iRange = 10;
    }
    else
    {
        if(SeAd760xRangeLow() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
        tAd760xModeLogger.iRange = 5;
    }

	return SE_RETURN_OK;
}

SeAd760xModeLogger SeAd760xReadMode(void)
{
    return tAd760xModeLogger;
}

SeInt8 SeAd760xInit(SeAd760xDescription tAd760xDescription)
{ 
    if(tAd760xDescription.iAd760xResetPinIndex == 0 || tAd760xDescription.iAd760xSclkPinIndex == 0 ||
	      tAd760xDescription.iAd760xCsPinIndex == 0 || tAd760xDescription.iAd760xDoutPinIndex == 0 ||
		  tAd760xDescription.iAd760xOs0PinIndex == 0 || tAd760xDescription.iAd760xOs1PinIndex == 0 ||
		  tAd760xDescription.iAd760xOs2PinIndex == 0 || tAd760xDescription.iAd760xRangePinIndex == 0 ||
	      tAd760xDescription.iAd760xConvertPinIndex == 0 || tAd760xDescription.iAd760xBusyPinIndex == 0)
	{
		return SE_RETURN_ERROR;
	}
    if(SeAd760xReset() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    /* Set sampling rate to 64 */
	if(SeAd760xModeSet(Ad760xOSampleMode6) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	/* AD760xRange1= +-10V;AD760xRange0 = +-5V */
    if(SeAd760xRangeSet(Ad760xRange1) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    
	return SE_RETURN_OK;
}

SeInt8 SeAd760xIsCheckOK(void)
{
    SeUInt32 iData = 0;
	
    if(SeAd760xGetSampleData(Ad760xChannel1, &iData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR; 
	}
    
    return SE_RETURN_OK;
}

SeInt8 SeAd760xStartConvert(void)
{
    if(SeAd760xConvertLow() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    SeAd760xConvertDelay();
    if(SeAd760xConvertHigh() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
}

static SeInt8 SeAd760xGetConvertData(SeUInt8 *pData, SeUInt8 nSize)
{
    SeInt16 i = 0;
    SeInt32 iTempData = 0;
	SeUInt8 bData = 0;
    
	if(pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
    /* Every time read 8 channel data */
    if(SeAd760xClkHigh() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    if(SeAd760xCsLow() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}    
#if ADC7606_USE
    for(i = 0; i < 16*8; i++)
    {
        if(SeAd760xClkLow() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
        SeAd760xClkLowDelay();
		SeAd760xReadDataOut(&bData);
        iTempData = bData & 0x01;       
        pData[i/8] = pData[i/8] | (iTempData << (7-i%8));       
        if(SeAd760xClkHigh() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
        SeAd760xClkHighDelay();                   
    }   
#else 
    for(i = 0;i < 18*8; i++)
    {
        if(SeAd760xClkLow() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
        SeAd760xClkLowDelay(); 
		SeAd760xReadDataOut(&bData);
        iTempData= bData & 0x01;
        pData[i%18] = pData[i%18] | (iTempData << (i/18));
        if(SeAd760xClkHigh() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
        SeAd760xClkHighDelay();                   
    }    
#endif   
    if(SeAd760xCsHigh() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
}

#define BIT(x) (1 << (x)) 
#define SETBITS(x,y) ((x) |= (y))
#define CLEARBITS(x,y) ((x) &= (~(y))) 
#define CLEARBIT(x,y) CLEARBITS((x), (BIT((y))))
#define SETBIT(x,y) SETBITS((x), (BIT((y))))
#define BITVAL(x,y) (((x)>>(y)) & 1) 

SeInt8 SeAd760xGetSampleData(SeAd760xChannelNumber tChannel, SeUInt32 *pData)
{
#if ADC7606_USE 
    SeUInt8 bGetData[16]={0};
#else
    /* AD7608 */
    SeUInt8 bGetData[18]={0};
#endif
    SeUInt8 bData = 0;
	SeInt32 i = 0;
    SeUInt32 iGetValue = 0;
	
	if(pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
    /* Start convert */
    if(SeAd760xStartConvert() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

    /* Wait for convert finish */
    *pData = 0;
    do{
        SeAd760xCheckDelay();
        i++;
        if(i > AD760X_CONVERT_TIMEOUT_COUNT)
        {
            SeAd760xCsHigh();
            return SE_RETURN_ERROR;
        }
		if(SeAd760xIsConvertOK(&bData) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
    }while(1 == bData);
    
    /* Get the convert data */
    memset(bGetData,0,sizeof(bGetData));
    SeAd760xGetConvertData(bGetData, sizeof(bGetData)/sizeof(bGetData[0]));
    
    /* Analyze data */
#if ADC7606_USE
    iGetValue = (bGetData[tChannel*2] << 8) + bGetData[tChannel*2+1];
    
#else
    //AD7608
    iGetValue = 0;
    for(i = 0;i < 18; i++)
    {
        if(BITVAL(bGetData[18-i-1],tChannel) == 1)
            SETBIT(iGetValue,i);
        else
            CLEARBIT(iGetValue,i);		
    }  
#endif
    *pData = iGetValue;
       
    return SE_RETURN_OK;
}

SeInt8 SeAd760xGetSampleVolt(SeAd760xChannelNumber tChannel, SeDouble *pfMillivolt)
{
    SeUInt32 iVolt = 0;
    SeInt32 ret;
    
	if(pfMillivolt == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd760xGetSampleData(tChannel, &iVolt) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
#if ADC7606_USE
	if(10 == tAd760xModeLogger.iRange)
    {
    	if( iVolt > 0x7fff)
    		*pfMillivolt =  -1.0 * (0x10000 - iVolt) *10000/32768;
    	else
    		*pfMillivolt = 1.0 * iVolt *10000/32768 ;
    }
    else
	{
		if( iVolt > 0x7fff)
			*pfMillivolt = -1.0 * (0x10000 - iVolt) *5000/32768;
		else
			*pfMillivolt = 1.0 * iVolt *5000/32768 ;
	}
#else
    if(10 == tAd760xModeLogger.iRange)
    {
    	if( iVolt > 0x01ffff)
    		*pfMillivolt = -1.0 * (0x40000 - iVolt) *10000/131072;
    	else
    		*pfMillivolt = 1.0 * iVolt *10000/131072 ;
    }
    else
    {
    	if( iVolt > 0x01ffff)
    		*pfMillivolt = -1.0 * (0x40000 - iVolt) *5000/131072;
    	else
    		*pfMillivolt = 1.0 * iVolt *5000/131072 ;
    }
#endif
    
    return SE_RETURN_OK;
}

