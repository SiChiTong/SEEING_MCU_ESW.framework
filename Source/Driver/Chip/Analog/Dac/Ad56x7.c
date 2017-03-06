/*
 * Ad56x7.c
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */

#include "Ad56x7.h"

static SeAd56x7Description tAd56x7Desc;

/* Write Read flag   */
#define AD56X7_WRITE_FLAG  0
#define AD56X7_READ_FLAG   1
/* DAC cmd [C2:C1:C0][A2:A1:A0] */
/* [C2:C1:C0]<<3 */
/* Write register cmd */
#define AD56X7_WT_REG_CMD			    (0x0 << 3)
/* Update register cmd */
#define AD56X7_UP_REG_CMD			    (0x1 << 3)
/* Write register and update all register cmd */
#define AD56X7_WT_UPDATE_ALL_REG_CMD    (0x2 << 3)
/* Write and update DAC channel cmd */
#define AD56X7_WT_UP_DAC_CMD            (0x3 << 3)
/* Power  cmd */
#define AD56X7_POWER_CMD                (0x4 << 3)
/* Reset cmd */
#define AD56X7_RESET_CMD                (0x5 << 3)
/* LDAC cmd */
#define AD56X7_LDAC_CMD                 (0x6 << 3)
/* Inside reference voltage set cmd  */
#define AD56X7_IREF_SETUP_CMD           (0x7 << 3)

/* Select ad56x7 channel   */
#define AD56X7_DAC_CHAN_ADDR_A  	    0x0
#define AD56X7_DAC_CHAN_ADDR_B  	    0x1
#define AD56X7_DAC_CAHN_ADDR_BOTH 	    0x7

/* LDAC Setup Command[DB1:DB0] */
/* LDAC=0\1 */
#define AD56X7_REG_LDAC_0		        0x0
/* LDAC=X */
#define AD56X7_REG_LDAC_1		        0x0
/* register reset to zero */
/* DAC and input shif register */
#define AD56X7_REG_RST_0		        0x0
/* DAC and input shif ¡¢LDAC\Power-down\IREF register */
#define AD56X7_REG_RST_1		        0x1

/* Power Up/Down Command[DB5:DB4] */
#define AD56X7_REG_POWER_MODE_NORMAL	(0x0 << 4)
#define AD56X7_REG_POWER_MODE_1K		(0x1 << 4)
#define AD56X7_REG_POWER_MODE_100K		(0x2 << 4)
#define AD56X7_REG_POWER_MODE_HIGHZ		(0x3 << 4)

/* Reference Setup Command */
#define AD56X7_REG_IREF_ON              0x1
#define AD56X7_REG_IREF_OFF             0x0

#define SeAd56x7Start()                     SeI2cStart(tAd56x7Desc.iAd56x7I2cIndex)
#define SeAd56x7Stop()                      SeI2cStop(tAd56x7Desc.iAd56x7I2cIndex)
#define SeAd56x7Ack()                       SeI2cAck(tAd56x7Desc.iAd56x7I2cIndex)
#define SeAd56x7Nack()                      SeI2cNack(tAd56x7Desc.iAd56x7I2cIndex)
#define SeAd56x7ReadByte(pData, blAck)      SeI2cRead(tAd56x7Desc.iAd56x7I2cIndex, pData, blAck)
#define SeAd56x7WriteByte(bData, blAck)     SeI2cWrite(tAd56x7Desc.iAd56x7I2cIndex, bData, blAck)

static SeInt8 SeAd56x7Read(SeUInt8 hAddress, SeUInt8 *pCmd, SeUInt8 *pHighBuf, SeUInt8 *pLowBuf)
{
	SeBool blSendAck = SeTrue;
	SeBool blGetAck = SeFalse;
		
	if((pCmd == SeNull) || (pHighBuf == SeNull) || (pLowBuf == SeNull))
	{
		return SE_RETURN_ERROR;
	}	
	if(SeAd56x7Start() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}		
	if((SeAd56x7WriteByte(hAddress|AD56X7_READ_FLAG, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
	{
		if(SeAd56x7Stop() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
		return SE_RETURN_ERROR;
	}	
	if(SeAd56x7ReadByte(pCmd, blSendAck) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd56x7ReadByte(pHighBuf, blSendAck) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd56x7ReadByte(pLowBuf, blSendAck) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAd56x7Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
		
	return SE_RETURN_OK;
}

static SeInt8 SeAd56x7Write(SeUInt8 hAddress, SeUInt8 bCmd, SeUInt8 bHigh,SeUInt8 bLow)
{
	SeBool blGetAck = SeFalse;

	if(SeAd56x7Start() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}		
	if((SeAd56x7WriteByte(hAddress, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
	{
		goto I2cCmdFail;	
	}
	if((SeAd56x7WriteByte(bCmd, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
	{
		goto I2cCmdFail;
	}
	if((SeAd56x7WriteByte(bHigh, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
	{
		goto I2cCmdFail;
	}
	if((SeAd56x7WriteByte(bLow, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
	{
		goto I2cCmdFail;
	}	
	if(SeAd56x7Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}	   	
	return SE_RETURN_OK;
I2cCmdFail:
	if(SeAd56x7Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR;
}

static SeInt8 SeAd56x7SoftReset(SeUInt8 hAddress, SeUInt8 iFlag)
{
    iFlag = iFlag>0?AD56X7_REG_RST_0:AD56X7_REG_RST_1; 
    if(SeAd56x7Write(hAddress, AD56X7_RESET_CMD, 0, iFlag) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    
   return SE_RETURN_OK;
}

static SeInt8 SeAd56x7SetReferenceVoltage(SeUInt8 hAddress, SeUInt8 iFlag)
{
    if(0 == iFlag)
	{
		iFlag = AD56X7_REG_IREF_OFF;
	}      
    else
	{
		iFlag = AD56X7_REG_IREF_ON;
	}       
    if(SeAd56x7Write(hAddress, AD56X7_RESET_CMD, 0, iFlag) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    
    return SE_RETURN_OK;
}

static SeInt8 SeAd56x7SetWorkMode(SeUInt8 hAddress, SeUInt8 iMode)
{
    if(SeAd56x7Write(hAddress, AD56X7_POWER_CMD, 0, iMode) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    
    return SE_RETURN_OK;
}

SeInt8 SeAd56x7ChipInit(SeAd56x7Description tAd56x7Description)
{   
	if(tAd56x7Description.iAd56x7I2cIndex == 0 || tAd56x7Description.bAd56x7DeviceAddress == 0)
	{
		return SE_RETURN_ERROR;
	}
    /* Reset ad56x7 */
	if(SeAd56x7SoftReset(tAd56x7Description.bAd56x7DeviceAddress, 1) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}     
    /* Select external reference voltage  */
    if(SeAd56x7SetReferenceVoltage(tAd56x7Description.bAd56x7DeviceAddress, 0) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    /* Set LDAC  */
    if(SeAd56x7Write(tAd56x7Description.bAd56x7DeviceAddress, AD56X7_LDAC_CMD , 0, 0x3) == SE_RETURN_ERROR)
    {
	    return SE_RETURN_ERROR;
    }
    /* Set ad56x7 work mode  */
    if(SeAd56x7SetWorkMode(tAd56x7Description.bAd56x7DeviceAddress, AD56X7_REG_POWER_MODE_NORMAL|0x3) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
    return SE_RETURN_OK;
}

SeInt8 SeAd56x7SetRegData(SeUInt8 hAddress, SeAd56x7ChannelNumber tChannel, SeUInt16 iData)
{
	SeUInt8 bCmd = 0;

	if(Ad56x7ChannelA == tChannel)
	{
		bCmd = AD56X7_WT_UP_DAC_CMD | AD56X7_DAC_CHAN_ADDR_A;
	}
	else if(Ad56x7ChannelB == tChannel)
	{
		bCmd = AD56X7_WT_UP_DAC_CMD | AD56X7_DAC_CHAN_ADDR_B;
	}
	else if(Ad56x7ChannelAB == tChannel)
	{
		bCmd = AD56X7_WT_UP_DAC_CMD | AD56X7_DAC_CAHN_ADDR_BOTH;
	}
	else
	{
		return SE_RETURN_ERROR;
	}
	
	if(SeAd56x7Write(hAddress, bCmd, (iData&0xff00) >> 8, iData&0xff) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    
    return SE_RETURN_OK;
}

SeInt8 SeAd56x7SetOutputVolt(SeUInt8 hAddress, SeAd56x7ChannelNumber tChannel, SeDouble fVolt)
{
    SeUInt16 iData;
	
    if(fVolt < 0)
	{
		 fVolt = 0;
	}   
    else if(fVolt > AD56X7_VREF)
	{
		fVolt = AD56X7_VREF;
	}
    iData = AD56X7_VoltToData(fVolt);
    if(SeAd56x7SetRegData(hAddress, tChannel, iData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
	
}


