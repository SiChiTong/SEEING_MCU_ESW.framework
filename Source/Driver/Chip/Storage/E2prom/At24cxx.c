/*
 * At24cxx.c
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */
 
#include "At24cxx.h"

void SeAt24cxxDelayMs(volatile SeUInt16 nMs)
{
	SeUInt16 iIndex;

    while((nMs--) > 0)
    {
        for(iIndex = 42000; iIndex > 0; --iIndex);
    }
}

#define AT24CXX_WRITE_SIGNAL							0
#define AT24CXX_READ_SIGNAL								1
#define SeAt24cxxE2promDelayMs(n)				SeAt24cxxDelayMs(n)
#define SeAt24cxxStart()											SeI2cStart(pAt24cxxDescription->iAt24cxxI2cIndex)
#define SeAt24cxxStop()											SeI2cStop(pAt24cxxDescription->iAt24cxxI2cIndex)
#define SeAt24cxxAck()												SeI2cAck(pAt24cxxDescription->iAt24cxxI2cIndex)
#define SeAt24cxxNack()											SeI2cNack(pAt24cxxDescription->iAt24cxxI2cIndex)
#define SeAt24cxxReadByte(bData, blAck)		SeI2cRead(pAt24cxxDescription->iAt24cxxI2cIndex, bData, blAck)
#define SeAt24cxxWriteByte(bData, blAck)		SeI2cWrite(pAt24cxxDescription->iAt24cxxI2cIndex, bData, blAck)

static SeInt8 SeAt24cxxE2promCheckIsOK(SeAt24cxxDescription* pAt24cxxDescription)
{
	SeBool blGetAck = SeFalse;
	
    if(SeAt24cxxStart() != SE_RETURN_OK)
	{
    	return SE_RETURN_ERROR;
	}
    if((SeAt24cxxWriteByte(0xaa, &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
	{
    	if(SeAt24cxxStop() != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
		return SE_RETURN_ERROR;
	}
    if(SeAt24cxxStop() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

static SeInt8 SeAt24cxxE2promGetPageSize(SeAt24cxxDescription* pAt24cxxDescription, SeUInt8* pPageSize)
{
	/* Get at24cxxe2prom page size */
	switch(pAt24cxxDescription->tAt24cxxType)
	{
		case At24c01:
		{
			*pPageSize = AT24C01_PAGE_SIZE;
			break;
		}			
		case At24c02:
		{
			*pPageSize = AT24C02_PAGE_SIZE;
			break;
		}				
		case At24c04:
		{
			*pPageSize = AT24C04_PAGE_SIZE;
			break;
		}				
		case At24c08:
		{
			*pPageSize = AT24C08_PAGE_SIZE;
			break;
		}			
		case At24c16:
		{
			*pPageSize = AT24C16_PAGE_SIZE;
			break;
		}			
		case At24c32:
		{
			*pPageSize = AT24C32_PAGE_SIZE;
			break;
		}			
		case At24c64:
		{
			*pPageSize = AT24C64_PAGE_SIZE;
			break;
		}			
		case At24c128:
		{
			*pPageSize = AT24C128_PAGE_SIZE;
			break;
		}			
		case At24c256:
		{
			*pPageSize = AT24C256_PAGE_SIZE;
			break;
		}			
		case At24c512:
		{
			*pPageSize = AT24C512_PAGE_SIZE;
			break;
		}			
		default:
			*pPageSize = AT24C08_PAGE_SIZE;
			break;		
	}
	return SE_RETURN_OK;	
}

SeInt8 SeAt24cxxE2promInit(SeAt24cxxDescription* pAt24cxxDescription)
{
	if(pAt24cxxDescription == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	if(pAt24cxxDescription->iAt24cxxI2cIndex < 0 || pAt24cxxDescription->tAt24cxxType < 0 || pAt24cxxDescription->hAt24cxxDeviceAddress == 0)
	{
		return SE_RETURN_ERROR;
	}
	if (SeAt24cxxE2promCheckIsOK(pAt24cxxDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeAt24cxxE2promReadData(SeAt24cxxDescription* pAt24cxxDescription, SeUInt16 hAddress, SeUInt8 *pBuffer, SeUInt16 nBytes)
 {
	SeUInt16 iIndex = 0;
	SeUInt8 hReadAddress = 0;
	SeBool blSendAck = SeFalse;
	SeBool blGetAck = SeFalse;
	
	if(pBuffer == SeNull || nBytes == 0)
	{
		return SE_RETURN_ERROR;
	}
	if((hAddress + nBytes) > pAt24cxxDescription->iAt24cxxChipSize - 1)
	{
		return SE_RETURN_ERROR;
	}
	if(SeAt24cxxStart() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(pAt24cxxDescription->tAt24cxxType > At24c16)
	{
		if((SeAt24cxxWriteByte(pAt24cxxDescription->hAt24cxxDeviceAddress|AT24CXX_WRITE_SIGNAL, &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
		/*
		 * write high 8bits address
		 */
		if((SeAt24cxxWriteByte((hAddress>>8)&0xff, &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
	}
	else
	{
		hReadAddress = (pAt24cxxDescription->hAt24cxxDeviceAddress|AT24CXX_WRITE_SIGNAL) | ((hAddress / 256) << 1);
		if((SeAt24cxxWriteByte(hReadAddress , &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
	}
	/*
	* write low 8bits address
	 */
	if((SeAt24cxxWriteByte((hAddress % 256), &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
	{
		goto I2cCmdFail;
	}

	if(SeAt24cxxStart() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if((SeAt24cxxWriteByte(pAt24cxxDescription->hAt24cxxDeviceAddress|AT24CXX_READ_SIGNAL, &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
	{
		goto I2cCmdFail;
	}
	for (iIndex = 0; iIndex < nBytes; iIndex++ )
	{
		{
			if(SeAt24cxxReadByte(&pBuffer[iIndex], blSendAck) != SE_RETURN_OK)
			{
				goto I2cCmdFail;
			}
			if (iIndex != nBytes-1)
			{
				if(SeAt24cxxAck() != SE_RETURN_OK)
				{
					goto I2cCmdFail;
				}
			}
			else
			{
				if(SeAt24cxxNack() != SE_RETURN_OK)
				{
					goto I2cCmdFail;
				}
			}			
		}
	}

    if(SeAt24cxxStop() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR; 
	}
	return SE_RETURN_OK;
	
I2cCmdFail:
	if(SeAt24cxxStop() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR; 
	}
	return SE_RETURN_ERROR; 
 }

SeInt8 SeAt24cxxE2promWriteData(SeAt24cxxDescription* pAt24cxxDescription, SeUInt16 hAddress, SeUInt8 *pBuffer, SeUInt16 nBytes)
{
	SeUInt16 iIndex = 0;
	SeUInt8 hPageAddress = 0;
	SeUInt8 nAt24cxxPageSize = 0;
	SeBool blGetAck = SeFalse;
	
	if(pBuffer == SeNull || nBytes == 0)
	{
		return SE_RETURN_ERROR;
	}
	if((hAddress + nBytes) > pAt24cxxDescription->iAt24cxxChipSize - 1)
	{
		return SE_RETURN_ERROR;
	}

	/*
	 * get At24cxx page size
	 */
	if(SeAt24cxxE2promGetPageSize(pAt24cxxDescription, &nAt24cxxPageSize) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}		
	for(iIndex = 0; iIndex < nBytes; iIndex++ )
	{
		if((iIndex == 0) || (hAddress & (nAt24cxxPageSize - 1)) == 0)
		{
            if(SeAt24cxxStop() != SE_RETURN_OK)
			{
				return SE_RETURN_ERROR;
			}
            /*
             * At24cxx each write cycle finsish need about 10ms
             */
            SeAt24cxxE2promDelayMs(20);
			if(SeAt24cxxStart() != SE_RETURN_OK)
			{
				return SE_RETURN_ERROR;
			}			
			if(pAt24cxxDescription->tAt24cxxType > At24c16)
			{
				if((SeAt24cxxWriteByte(pAt24cxxDescription->hAt24cxxDeviceAddress|AT24CXX_WRITE_SIGNAL, &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
				{
					goto I2cCmdFail;
				}
				/*
				 * write high 8bits address
				 */
				if((SeAt24cxxWriteByte((hAddress>>8)&0xff, &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
				{
					goto I2cCmdFail;
				}
			}
			else
			{
				hPageAddress = (pAt24cxxDescription->hAt24cxxDeviceAddress | AT24CXX_WRITE_SIGNAL) | ((hAddress / 256) << 1);
				if((SeAt24cxxWriteByte(hPageAddress , &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
				{
					goto I2cCmdFail;
				}
			}
			/*
			 * write low 8bits address
			 */
			if((SeAt24cxxWriteByte((hAddress % 256), &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
			{
				goto I2cCmdFail;
			}
		}
		/*
		 * write one byte data
		 */
		if((SeAt24cxxWriteByte(pBuffer[iIndex], &blGetAck) != SE_RETURN_OK) || (blGetAck == SeFalse))
		{
			goto I2cCmdFail;
		}
		hAddress++;
	}
    if(SeAt24cxxStop() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
	
I2cCmdFail:
	if(SeAt24cxxStop() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_ERROR; 
}

