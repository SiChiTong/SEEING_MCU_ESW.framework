/*
 * At24cxx_Eeprom.c
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */
 
#include "Tca9548.h"

static SeTca9548Description tTca9548Desc;

#define TCA9548_ADD                          ((0x70|0x01)<<1)
#define TCA9548_READ_FLAG                    0x01
#define SeTca9548Start()                     SeI2cStart(tTca9548Desc.iTca9548I2cIndex)
#define SeTca9548Stop()                      SeI2cStop(tTca9548Desc.iTca9548I2cIndex)
#define SeTca9548Ack()                       SeI2cAck(tTca9548Desc.iTca9548I2cIndex)
#define SeTca9548Nack()                      SeI2cNack(tTca9548Desc.iTca9548I2cIndex)
#define SeTca9548ReadByte(pData, blAck)      SeI2cRead(tTca9548Desc.iTca9548I2cIndex, pData, blAck)
#define SeTca9548WriteByte(bData, blAck)     SeI2cWrite(tTca9548Desc.iTca9548I2cIndex, bData, blAck)

SeInt8 SeTca9548Init(SeTca9548Description tTca9548Description)
{
    if(tTca9548Description.iTca9548I2cIndex == 0 || tTca9548Description.bTca9548DeviceAddress == 0)
	{
		return SE_RETURN_ERROR;
	}
	
	return SE_RETURN_OK;
}

SeInt8 SeTca9548Config(SeUInt8 hAddess, SeUInt8 iChannelx)
{
    SeBool blGetAck = SeFalse;
	
	if(SeTca9548Start() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}	 
    if((SeTca9548WriteByte(hAddess, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
	{
		goto I2cCmdFail;
	}	
   /* eg:iChannelx=0x03, select 1 and 2 channel */
    if((SeTca9548WriteByte(iChannelx, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse)) 
	{
		goto I2cCmdFail;
	}	
    if(SeTca9548Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    return SE_RETURN_OK;
I2cCmdFail:
	if(SeTca9548Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR;
}

SeInt8 SeTca9548ConfigRead(SeUInt8 hAddess, SeUInt8 *pChannelx)
{
    SeBool blSendAck = SeFalse;
	SeBool blGetAck = SeFalse;
	
	if(SeTca9548Start() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}	   
    if((SeTca9548WriteByte(hAddess|TCA9548_READ_FLAG, &blGetAck) <= 0) || (blGetAck == SeFalse)) 
	{
		goto I2cCmdFail;
	}
	/* eg:chanx=0x03, select 1 and 2 channel */
	if(SeTca9548ReadByte(pChannelx, blSendAck) == SE_RETURN_ERROR)  
	{
		goto I2cCmdFail;
	}
    if(SeTca9548Nack() == SE_RETURN_ERROR) 
	{
		goto I2cCmdFail;
	}
    if(SeTca9548Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}   
    return SE_RETURN_OK;
I2cCmdFail:
	if(SeTca9548Stop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR;	
}

SeInt8 SeTca9548SelectOneChannel(SeUInt8 iChanel)
{
    SeUInt8 iTemp = 0;
    
	if(SeTca9548ConfigRead(TCA9548_ADD|TCA9548_READ_FLAG, &iTemp) == SE_RETURN_ERROR)
    {
       return SE_RETURN_ERROR;
    }
	iTemp |= (0x01<<iChanel);
    if(SeTca9548Config(TCA9548_ADD, iTemp) == SE_RETURN_ERROR)
    {
        return SE_RETURN_ERROR;
    }
	
    return SE_RETURN_OK;
}

SeInt8 SeTca9548CloseOneChan(SeUInt8 iChanel)
{
    SeUInt8 iTemp = 0;
	
    if(SeTca9548ConfigRead(TCA9548_ADD|TCA9548_READ_FLAG, &iTemp) == SE_RETURN_ERROR)
    {
       return SE_RETURN_ERROR;
    }
	
    iTemp &= ~(0x01<<iChanel);
	
    if(SeTca9548Config(TCA9548_ADD, iTemp) == SE_RETURN_ERROR)
    {
        return SE_RETURN_ERROR;
    }
	
    return SE_RETURN_OK;
}

SeInt8 SeTca9548OpenOneChan(SeUInt8 iChanel)
{
    SeUInt8 iTemp = 0;
	
	iTemp = 0x01 << iChanel;
   if(SeTca9548Config(TCA9548_ADD, iTemp) == SE_RETURN_ERROR)
    {
        return SE_RETURN_ERROR;
    }
    return SE_RETURN_OK;
}

SeInt8 SeTca9548ConfigAllChan(SeUInt8 bChanelConfig)
{
    if(SeTca9548Config(TCA9548_ADD, bChanelConfig) == SE_RETURN_ERROR)
    {
        return SE_RETURN_ERROR;
    }
	
    return SE_RETURN_OK;
}

