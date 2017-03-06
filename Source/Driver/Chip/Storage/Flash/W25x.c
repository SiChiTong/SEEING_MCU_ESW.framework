/*
 * W25x.c
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#include "W25x.h"

/*
 * W25x spi flash operate cmd code
 */
#define W25X_WRITE_ENABLE                             0x06
#define W25X_WRITE_DISABLE                            0x04
#define W25X_READ_STATUS_REGISTER            0x05
#define W25X_WRITE_STATUS_REGISTER          0x01
#define W25X_READ_DATA                                     0x03
#define W25X_FAST_READ_DATA                          0x0B
#define W25X_FAST_READ_DUAL                         0x3B
#define W25X_PAGE_PROGRAM                           0x02
#define W25X_SECTOR_ERASE                              0x20
#define W25X_BLOCK_ERASE                                0xD8
#define W25X_CHIP_ERASE                                    0xC7
#define W25X_POWER_DOWN                              0xB9
#define W25X_RELEASE_POWER_DOWN           0xAB
#define W25X_DEVICE_ID                                       0xAB
#define W25X_MANUFACT_DEVICE_ID               0x90
#define W25X_JEDEC_DEVICE_ID                          0x9F
#define W25X_DUMMY_BYTE                                 0xA5

#define SeW25xWpLow()								SeGpioSetBit(pW25xDescription->iW25xWriteProtectPinIndex, SE_BIT_LOW)
#define SeW25xWpHigh()								SeGpioSetBit(pW25xDescription->iW25xWriteProtectPinIndex, SE_BIT_HIGH)
#define SeW25xCsLow()								SeGpioSetBit(pW25xDescription->iW25xCsPinIndex, SE_BIT_LOW)
#define SeW25xCsHigh()								SeGpioSetBit(pW25xDescription->iW25xCsPinIndex, SE_BIT_HIGH)
#define SeW25xWriteByte(bData)				SeSpiWrite(pW25xDescription->iW25xSpiIndex, bData)
#define SeW25xReadByte(bData)				SeSpiRead(pW25xDescription->iW25xSpiIndex, bData)

SeInt8 SeW25xSpiFlashInit(SeW25xDescription* pW25xDescription)
{ 	
	if(pW25xDescription == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	if(pW25xDescription->iW25xSpiIndex < 0 || pW25xDescription->iW25xWriteProtectPinIndex < 0 || pW25xDescription->iW25xCsPinIndex < 0)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWpLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
    return SE_RETURN_OK;
}

static SeInt8 SeW25xSpiFlashWriteEnable(SeW25xDescription* pW25xDescription)
{
	if(SeW25xWpHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_WRITE_ENABLE) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

static SeInt8 SeW25xSpiFlashWriteDisable(SeW25xDescription* pW25xDescription)
{
	if(SeW25xWpLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_WRITE_DISABLE)  != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashReadStatusRegister(SeW25xDescription* pW25xDescription, SeUInt8 *pState)
{		
	if(pState == SeNull)
	{
		return SE_RETURN_ERROR;
	}	
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_READ_STATUS_REGISTER)  != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xReadByte(pState) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashWaitForIdle(SeW25xDescription* pW25xDescription)
{
	SeUInt8 bState = 0;

	do{
		if(SeW25xSpiFlashReadStatusRegister(pW25xDescription, &bState) != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
	}while((bState & 0x01) == 0x01);
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashReadData(SeW25xDescription* pW25xDescription, SeUInt32 hAddress, SeUInt8 *pBuffer, SeUInt32 nBytes)
{
	SeUInt8 *pData = SeNull;
	
	if(pBuffer == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		pData = pBuffer;
	}
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_READ_DATA)  != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	/* 24 bits address */
	if(SeW25xWriteByte((hAddress & 0xff0000) >> 16) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte( (hAddress & 0xff00) >> 8) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(hAddress & 0xff) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	while(nBytes--)
	{
		if(SeW25xReadByte(pData) != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
		pData++;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashFastReadData(SeW25xDescription* pW25xDescription, SeUInt32 hAddress, SeUInt8 *pBuffer, SeUInt32 nBytes)
{
	SeUInt8 *pData = SeNull;
	
	if(pBuffer == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		pData = pBuffer;
	}
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_FAST_READ_DATA)  != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	/* 24 bits address */
	if(SeW25xWriteByte((hAddress & 0xff0000) >> 16) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte((hAddress & 0xff00) >> 8) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(hAddress & 0xff) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_DUMMY_BYTE) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	while(nBytes--)
	{
		if(SeW25xReadByte(pData) != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
		pData++;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

static SeInt8 SeW25xSpiFlashPageProgram(SeW25xDescription* pW25xDescription, SeUInt8 *pBuffer, SeUInt32 hAddress, SeUInt32 nByte, SeUInt32 *pRemainByte)
{
	SeUInt8 *pData = SeNull;
	SeUInt32 nCount = 0;
	
	if(pBuffer == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		pData = pBuffer;
	}
	if(pRemainByte == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		*pRemainByte = 0;
	}
	if(((hAddress % W25X_PAGE_SIZE) + nByte) > W25X_PAGE_SIZE)
	{
		nCount = W25X_PAGE_SIZE - (hAddress % W25X_PAGE_SIZE);
		*pRemainByte = nByte - nCount;
	}
	else
	{
		nCount = nByte;
	}
	if(SeW25xSpiFlashWriteEnable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_PAGE_PROGRAM) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	/* 24 bits address */
	if(SeW25xWriteByte((hAddress & 0xff0000) >> 16) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte((hAddress & 0xff00) >> 8) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(hAddress & 0xff) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	while((nCount--) > 0)
	{
		if(SeW25xWriteByte(*pData) != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
		pData++;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xSpiFlashWaitForIdle(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	if(SeW25xSpiFlashWriteDisable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashSectorErase(SeW25xDescription* pW25xDescription, SeUInt32 hAddress)
{
	if(SeW25xSpiFlashWriteEnable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_SECTOR_ERASE) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	if(SeW25xWriteByte((hAddress & 0xff0000) >> 16) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte((hAddress & 0xff00) >> 8) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(hAddress & 0xff) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xSpiFlashWaitForIdle(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	if(SeW25xSpiFlashWriteDisable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashBlockErase(SeW25xDescription* pW25xDescription, SeUInt32 hAddress)
{
	if(SeW25xSpiFlashWriteEnable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_BLOCK_ERASE) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	if(SeW25xWriteByte((hAddress & 0xff0000) >> 16) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte((hAddress & 0xff00) >> 8) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(hAddress & 0xff) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xSpiFlashWaitForIdle(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	if(SeW25xSpiFlashWriteDisable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashChipErase(SeW25xDescription* pW25xDescription, SeUInt8 iEraseMode)
{
	SeUInt32 hAddress = 0x00;

	if(SeW25xSpiFlashWriteEnable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(iEraseMode > 1)
	{
		if(SeW25xCsLow() != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
		if(SeW25xWriteByte(W25X_CHIP_ERASE) != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
		if(SeW25xCsHigh() != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
		if(SeW25xSpiFlashWaitForIdle(pW25xDescription) != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
	}
	else
	{
		for(hAddress = 0; hAddress < W25X_CHIP_SIZE; )
		{
			if(SeW25xSpiFlashBlockErase(pW25xDescription, hAddress) != SE_RETURN_OK)
			{
				return SE_RETURN_ERROR;
			}
			hAddress += W25X_BLOCK_SIZE;
		}
	}
    if(SeW25xSpiFlashWriteDisable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;	
}

SeInt8 SeW25xSpiFlashPowerDown(SeW25xDescription* pW25xDescription)
{
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_POWER_DOWN) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashWakeUp(SeW25xDescription* pW25xDescription)
{	
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_RELEASE_POWER_DOWN) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xFlashReadDeviceID(SeW25xDescription* pW25xDescription, SeUInt8 *pId)
{
	if(pId == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	*pId = 0;
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_DEVICE_ID) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_DUMMY_BYTE) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_DUMMY_BYTE) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_DUMMY_BYTE) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xReadByte(pId) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashReadManufactDeviceID(SeW25xDescription* pW25xDescription, SeUInt16 *pId)
{
	SeUInt8 bTemp[2] = {0};
	SeUInt8 iIndex = 0;
	
	if(pId == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	*pId = 0;
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(W25X_MANUFACT_DEVICE_ID) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	if(SeW25xWriteByte(0x00) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(0x00) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xWriteByte(0x00) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	for(iIndex = 0;iIndex < (sizeof(bTemp)/sizeof(bTemp[0])); iIndex++)
	{
		if(SeW25xReadByte(&bTemp[iIndex]) != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	for(iIndex = 0; iIndex < (sizeof(bTemp)/sizeof(bTemp[0])); iIndex++)
	{
		*pId = *pId << 8;
		*pId |= bTemp[iIndex];
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashReadJedecDeviceID(SeW25xDescription* pW25xDescription, SeUInt32 *pId)
{
	SeUInt8 bTemp[3] = {0};
	SeUInt8 iIndex = 0;

	if(pId == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	*pId = 0;
	if(SeW25xCsLow() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	if(SeW25xReadByte(W25X_JEDEC_DEVICE_ID) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}	
	for(iIndex = 0; iIndex < (sizeof(bTemp)/sizeof(bTemp[0])); iIndex++)
	{
		if(SeW25xReadByte(&bTemp[iIndex]) != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
	}
	if(SeW25xCsHigh() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	for(iIndex = 0; iIndex < (sizeof(bTemp)/sizeof(bTemp[0]));iIndex++)
	{
		*pId = *pId << 8;
		*pId |= bTemp[iIndex];
	}
	return SE_RETURN_OK;
}

SeInt8 SeW25xSpiFlashWriteData(SeW25xDescription* pW25xDescription, SeUInt32 hAddress, SeUInt8 *pBuffer, SeUInt32 nBytes)
{ 
	SeUInt32 hWriteAddress = 0;
	SeUInt32 nCount = 0;
	SeUInt32 nRemainBytes = 0;
	SeUInt8 *pData = SeNull;
	
	if(pBuffer == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	else
	{
		pData = pBuffer;
	}	
	if(SeW25xSpiFlashWriteEnable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}   
	hWriteAddress = hAddress;
	nCount = nBytes;
	do
	{
		if(SeW25xSpiFlashPageProgram(pW25xDescription, pData, hWriteAddress, nCount, &nRemainBytes) != SE_RETURN_OK)
		{
			return SE_RETURN_ERROR;
		}
		if(nRemainBytes > 0)
		{
			pData += (nCount - nRemainBytes);
			hWriteAddress += (nCount - nRemainBytes);
			nCount = nRemainBytes;
		}
	}while(nRemainBytes > 0);
	if(SeW25xSpiFlashWriteEnable(pW25xDescription) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}


