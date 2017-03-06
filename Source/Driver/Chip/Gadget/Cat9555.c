/*
 * Cat9555.c
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */

#include "Cat9555.h"


#define CAT9555_INPUT_REGISTER_LOW8BITS							0x00
#define CAT9555_INPUT_REGISTER_HIGH8BITS							0x01
#define CAT9555_OUTPUT_REGISTER_LOW8BITS						0x02
#define CAT9555_OUTPUT_REGISTER_HIGH8BITS						0x03
#define CAT9555_INVERSION_REGISTER_LOW8BITS					0x04
#define CAT9555_INVERSION_REGISTER_HIGH8BITS				0x05
#define CAT9555_CONFIG_REGISTER_LOW8BITS						0x06
#define CAT9555_CONFIG_REGISTER_HIGH8BITS						0x07
#define CAT9555_CONFIG_OUTPUT													0x0000
#define CAT9555_CONFIG_INPUT														0xFFFF

#define SeCat9555Start()												SeI2cStart(pCat9555Description->iCat9555I2cIndex)
#define SeCat9555Stop()												SeI2cStop(pCat9555Description->iCat9555I2cIndex)
#define SeCat9555ACK()												SeI2cAck(pCat9555Description->iCat9555I2cIndex)
#define SeCat9555NACK()											SeI2cNack(pCat9555Description->iCat9555I2cIndex)
#define SeCat9555ReadByte(pData, blAck)			SeI2cRead(pCat9555Description->iCat9555I2cIndex, pData, blAck)
#define SeCat9555WriteByte(bData, blAck)			 SeI2cWrite(pCat9555Description->iCat9555I2cIndex, bData, blAck)

SeInt8 SeCat9555Init(SeCat9555Description* pCat9555Description)
{
    if(pCat9555Description->iCat9555I2cIndex < 0 || pCat9555Description->hCat9555DeviceAddress == 0)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

static SeInt8 SeCat9555WriteData(SeCat9555Description* pCat9555Description, SeUInt8 hRegAddress, SeUInt8 *pData, SeUInt8 nLength)
{
    SeUInt8 i = 0;
	SeBool blGetAck = SeFalse;

	if(pCat9555Description == SeNull || pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
    if(SeCat9555Start() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    if((SeCat9555WriteByte(pCat9555Description->hCat9555DeviceAddress, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
    {
        goto I2cCmdFail;
    }
    if((SeCat9555WriteByte(hRegAddress, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
    {
        goto I2cCmdFail;
    }
    for(i=0; i<nLength; i++)
    {
        if((SeCat9555WriteByte(pData[i], &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
        {
            goto I2cCmdFail;
        }  
    }
    if(SeCat9555Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}   
    return SE_RETURN_OK;
I2cCmdFail:
	if(SeCat9555Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR;
}

static SeInt8 SeCat9555ReadData(SeCat9555Description* pCat9555Description, SeUInt8 hRegAddress, SeUInt8 *pData, SeUInt8 nLength)
{
    SeUInt8 i = 0;
	SeBool blSendAck = SeFalse;
	SeBool blGetAck = SeFalse;

	if(pCat9555Description == SeNull || pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(SeCat9555Start() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}	 
    if((SeCat9555WriteByte(pCat9555Description->hCat9555DeviceAddress, &blGetAck) == SE_RETURN_ERROR) | (blGetAck == SeFalse))
    {
        goto I2cCmdFail;
    }
    if((SeCat9555WriteByte(hRegAddress, &blGetAck) == SE_RETURN_ERROR) | (blGetAck == SeFalse)) 
    {
        goto I2cCmdFail;
    }
    
    if(SeCat9555Start() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}	
    if((SeCat9555WriteByte(pCat9555Description->hCat9555DeviceAddress|0x01, &blGetAck) == SE_RETURN_ERROR) | (blGetAck == SeFalse))
    {
        goto I2cCmdFail;
    }

    for(i=0; i<nLength; i++)
    {
        if(SeCat9555ReadByte(&pData[i], blSendAck) == SE_RETURN_ERROR)
		{
			goto I2cCmdFail;
		}
        if(i == nLength-1)
        {
            if(SeCat9555NACK() == SE_RETURN_ERROR)
			{
				goto I2cCmdFail;
			}
        }
        else
        {
            if(SeCat9555ACK() == SE_RETURN_ERROR)
			{
				goto I2cCmdFail;
			}
        }
    }
    
    if(SeCat9555Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}   
    return SE_RETURN_OK;
I2cCmdFail:
	if(SeCat9555Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR;
}

SeInt8 SeCat9555InputOutputConfig(SeCat9555Description* pCat9555Description, SeUInt16 iStatus)
{
    SeUInt8 bTemp[2] = {0};

    if(pCat9555Description == SeNull)
	{
		return SE_RETURN_ERROR;
	}

    bTemp[0] = iStatus & 0xff;
    bTemp[1] = (iStatus>>8) & 0xff;
    if(SeCat9555WriteData(pCat9555Description, CAT9555_CONFIG_REGISTER_LOW8BITS, bTemp, 2) == SE_RETURN_ERROR)
	{
    	return SE_RETURN_ERROR;
	}		

    return SE_RETURN_OK;
}

SeInt8 SeCat9555ReadInputPortData(SeCat9555Description* pCat9555Description, SeUInt16 *pData)
{
    SeUInt8 bTemp[2] = {0};
 
    if(pCat9555Description == SeNull || pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		*pData = 0;
	}    
    if(SeCat9555ReadData(pCat9555Description, CAT9555_INPUT_REGISTER_LOW8BITS, bTemp, 2) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    else
	{
		*pData = (bTemp[1]<<8) | bTemp[0];
	}
      
    return SE_RETURN_OK;
}

SeInt8 SeCat9555WriteOutPortData(SeCat9555Description* pCat9555Description, SeUInt16 iData)
{
    SeUInt8 bTemp[2] = {0};

	if(pCat9555Description == SeNull)
	{
		return SE_RETURN_ERROR;
	}
    bTemp[0] = iData & 0xff;
    bTemp[1] = (iData>>8) & 0xff;
    
    if(SeCat9555WriteData(pCat9555Description, CAT9555_OUTPUT_REGISTER_LOW8BITS, bTemp, 2) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}		

    return SE_RETURN_OK;
}

SeInt8 SeCat9555ReadOutPortRegistorValue(SeCat9555Description* pCat9555Description, SeUInt16 *pData)
{    
    SeUInt8 bTemp[2] = {0};
    
	if(pCat9555Description == SeNull || pData == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		*pData = 0;
	}        
    if(SeCat9555ReadData(pCat9555Description, CAT9555_OUTPUT_REGISTER_LOW8BITS, bTemp, 2) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		*pData = (bTemp[1]<<8) | bTemp[0];
	}
    
    return SE_RETURN_OK;
}

SeInt8 SeCat9555WriteInputPortInversion(SeCat9555Description* pCat9555Description, SeUInt16 iData)
{
    SeUInt8 bTemp[2] = {0};

    if(pCat9555Description == SeNull)
    {
    	return SE_RETURN_ERROR;
 	 }
    bTemp[0] = iData & 0xff;
    bTemp[1] = (iData>>8) & 0xff;
    
    if(SeCat9555WriteData(pCat9555Description, CAT9555_INVERSION_REGISTER_LOW8BITS, bTemp, 2) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}		

    return SE_RETURN_OK;
}

SeInt8 SeCat9555ReadInputPinNumStatus(SeCat9555Description* pCat9555Description, SeUInt8 iPinIndex, SeUInt8 *pStatus)
{
    SeUInt16 iData = 0;

    if(pCat9555Description == SeNull || pStatus ==  SeNull)
    {
    	return SE_RETURN_ERROR;
 	 }
    if(SeCat9555ReadInputPortData(pCat9555Description, &iData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    else
	{
		 *pStatus = (iData>>iPinIndex) & 0x1;
	}
        
    return SE_RETURN_OK; 
}

SeInt8 SeCat9555WriteOutPinNumStatus(SeCat9555Description* pCat9555Description, SeUInt8 iPinIndex, SeUInt8 iStatus)
{
    SeUInt16 iData = 0;

    if(pCat9555Description == SeNull)
    {
    	return SE_RETURN_ERROR;
 	 }
    if(SeCat9555ReadOutPortRegistorValue(pCat9555Description, &iData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    else
    {
		if(iStatus)
		{
			iData |= (0x0001 << iPinIndex);
		}
		else
		{
			iData &= ~(0x0001 << iPinIndex);
		}
				
		if(SeCat9555WriteOutPortData(pCat9555Description, iData) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
    }
	
    return SE_RETURN_OK; 
}

SeInt8 SeCat9555PinsSetLow(SeCat9555Description* pCat9555Description, SeUInt16 iPinx)
{
    SeUInt16 iData = 0;

    if(pCat9555Description == SeNull)
    {
    	return SE_RETURN_ERROR;
    }
    if(SeCat9555ReadOutPortRegistorValue(pCat9555Description, &iData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    else
    {
    	iData &= (~iPinx);
        if(SeCat9555WriteOutPortData(pCat9555Description, iData) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
    }
	
    return SE_RETURN_OK; 
}

SeInt8 SeCat9555PinsSetHigh(SeCat9555Description* pCat9555Description, SeUInt16 iPinx)
{
    SeUInt16 iData = 0;

    if(pCat9555Description == SeNull)
   	{
   		return SE_RETURN_ERROR;
   	}
    if(SeCat9555ReadOutPortRegistorValue(pCat9555Description, &iData) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    else
    {
    	iData |= iPinx;
        if(SeCat9555WriteOutPortData(pCat9555Description, iData) == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
    }
	
    return SE_RETURN_OK; 
}
