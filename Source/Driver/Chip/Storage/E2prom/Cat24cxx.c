/*
 * Cat24cxx.c
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#include "Cat24cxx.h"

#define CAT24CXX_WRITE_SIGNAL                 0
#define CAT24CXX_READ_SIGNAL                   1
#define SeCat24cxxE2promDelayMs(n)       SeSystemDelayMs(n)

#define SeCat24cxxE2promI2cStart()                       						SeI2cStart(pCat24cxxDescription->iCat24cxxI2cIndex)
#define SeCat24cxxE2promI2cStop()                       						SeI2cStop(pCat24cxxDescription->iCat24cxxI2cIndex)
#define SeCat24cxxE2promI2cAck()                        						 	SeI2cAck(pCat24cxxDescription->iCat24cxxI2cIndex)
#define  SeCat24cxxE2promI2cNack()                     						 SeI2cNack(pCat24cxxDescription->iCat24cxxI2cIndex)
#define  SeCat24cxxE2promI2cReadByte(pData, blAck)			SeI2cRead(pCat24cxxDescription->iCat24cxxI2cIndex, pData, blAck)
#define  SeCat24cxxE2promI2cWriteByte(bData, blAck)			SeI2cWrite(pCat24cxxDescription->iCat24cxxI2cIndex, bData, blAck)

SeInt8  SeCat24cxxE2promInit(SeCat24cxxDescription* pCat24cxxDescription)
{
	if(pCat24cxxDescription->iCat24cxxI2cIndex < 0 || pCat24cxxDescription->tCat24cxxType < 0 || pCat24cxxDescription->hCat24cxxDeviceAddress == 0)
	{
		return SE_RETURN_ERROR;
	}
	if (SE_RETURN_OK == SeCat24cxxE2promCheckIsOK(pCat24cxxDescription))
	{
		return SE_RETURN_OK;
	}
	else
	{
        return SE_RETURN_ERROR;
	}
}

SeInt8 SeCat24cxxE2promCheckIsOK(SeCat24cxxDescription* pCat24cxxDescription)
{
	SeBool blGetAck = SeFalse;

    if(SeCat24cxxE2promI2cStart() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
    if((SeCat24cxxE2promI2cWriteByte(0xaa, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
	{
		if(SeCat24cxxE2promI2cStop() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
		return SE_RETURN_ERROR;
	}

    if(SeAt24cxxE2promI2cStop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeCat24cxxE2promReadData(SeCat24cxxDescription* pCat24cxxDescription, SeUInt16 hAddress, SeUInt8 *pBuffer, SeUInt16 nBytes)
 {
	SeUInt16 iIndex = 0;
	SeUInt8 hReadAddress = 0;
	SeBool blSendAck = SeFalse;
	SeBool blGetAck = SeFalse;

	if(pBuffer == SeNull || nBytes == 0)
	{
		return SE_RETURN_ERROR;
	}
	if((hAddress + nBytes) > pCat24cxxDescription->iCat24cxxChipSize - 1)
	{
		return SE_RETURN_ERROR;
	}
	if(SeCat24cxxE2promI2cStart() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(pCat24cxxDescription->tCat24cxxType > Cat24c02)
	{
		hReadAddress = pCat24cxxDescription->hCat24cxxDeviceAddress | ((hAddress>>8) & 0x07) << 1;
		if((SeAt24cxxE2promI2cWriteByte(hReadAddress |CAT24CXX_WRITE_SIGNAL , &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
		if((SeAt24cxxE2promI2cWriteByte((hAddress & 0xff), &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
		if(SeCat24cxxE2promI2cStart() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
		if((SeCat24cxxE2promI2cWriteByte(hReadAddress|CAT24CXX_READ_SIGNAL, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
	}
	else
	{
		if((SeCat24cxxE2promI2cWriteByte(pCat24cxxDescription->hCat24cxxDeviceAddress|CAT24CXX_WRITE_SIGNAL, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
		if((SeAt24cxxE2promI2cWriteByte((hAddress & 0xff), &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
		if(SeCat24cxxE2promI2cStart() == SE_RETURN_ERROR)
		{
			return SE_RETURN_ERROR;
		}
		if((SeCat24cxxE2promI2cWriteByte(pCat24cxxDescription->hCat24cxxDeviceAddress|CAT24CXX_READ_SIGNAL, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
	}
	for (iIndex = 0; iIndex < nBytes; iIndex++ )
	{
		{
			if(SeCat24cxxE2promI2cReadByte(&pBuffer[iIndex], blSendAck) == SE_RETURN_ERROR)
			{
				goto I2cCmdFail;
			}
			if (iIndex != nBytes-1)
			{
				if(SeCat24cxxE2promI2cAck() == SE_RETURN_ERROR)
				{
					goto I2cCmdFail;
				}
			}
			else
			{
				if(SeCat24cxxE2promI2cNack() == SE_RETURN_ERROR)
				{
					goto I2cCmdFail;
				}
			}
		}
	}

    if(SeAt24cxxE2promI2cStop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;

I2cCmdFail:
	if(SeAt24cxxE2promI2cStop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR;
 }

SeInt8 SeCat24cxxE2promWriteData(SeCat24cxxDescription* pCat24cxxDescription, SeUInt16 hAddress, SeUInt8 *pBuffer, SeUInt16 nBytes)
{
	SeUInt16 iIndex = 0;
	SeUInt8 hPageAddress = 0;
	SeBool blGetAck = SeFalse;

	if(pBuffer == SeNull || nBytes == 0)
	{
		return SE_RETURN_ERROR;
	}
	if((hAddress + nBytes) > pCat24cxxDescription->iCat24cxxChipSize - 1)
	{
		return SE_RETURN_ERROR;
	}

	for(iIndex = 0; iIndex < nBytes; iIndex++ )
	{
		if((iIndex == 0) || (hAddress & (16 - 1)) == 0)
		{
            if(SeCat24cxxE2promI2cStop() == SE_RETURN_ERROR)
			{
				return SE_RETURN_ERROR;
			}
            /*
             * Cat24cxx each write cycle finsish need about 10ms
             */
            SeCat24cxxE2promDelayMs(10);
			if(SeCat24cxxE2promI2cStart() == SE_RETURN_ERROR)
			{
				return SE_RETURN_ERROR;
			}
			if(pCat24cxxDescription->tCat24cxxType > Cat24c02)
			{
				hPageAddress = pCat24cxxDescription->hCat24cxxDeviceAddress | ((hAddress>>8) & 0x07) << 1;
				if((SeCat24cxxE2promI2cWriteByte(hPageAddress | CAT24CXX_WRITE_SIGNAL, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
				{
					goto I2cCmdFail;
				}
				if((SeCat24cxxE2promI2cWriteByte(hAddress & 0xff, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
				{
					goto I2cCmdFail;
				}
			}
			else
			{
				if((SeCat24cxxE2promI2cWriteByte(pCat24cxxDescription->hCat24cxxDeviceAddress|CAT24CXX_WRITE_SIGNAL, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
				{
					goto I2cCmdFail;
				}
				if((SeCat24cxxE2promI2cWriteByte(hAddress & 0xff, &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
				{
					goto I2cCmdFail;
				}
			}
		}
		/*
		 * write one byte data
		 */
		if((SeCat24cxxE2promI2cWriteByte(pBuffer[iIndex], &blGetAck) == SE_RETURN_ERROR) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
		hAddress++;
	}
    if(SeCat24cxxE2promI2cStop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;

I2cCmdFail:
	if(SeCat24cxxE2promI2cStop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR;
}

