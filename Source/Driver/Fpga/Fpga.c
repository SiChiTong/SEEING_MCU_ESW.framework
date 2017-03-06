/*
 * Fpga.c
 *
 *  Created on: 2016年1月5日
 *      Author: seeing
 */

#include "Fpga.h"
#include <string.h>

#define SE_FPGA_PACKAGE_WR_MASK												0x80
#define SE_FPGA_PACKAGE_DIRECTION_MASK								0x40
#define SE_FPGA_PACKAGE_LENGTH_MASK										0x3F

#define SE_FPGA_HEAD_READ_FLAG														1
#define SE_FPGA_HEAD_WRITE_FLAG													0

#define SE_FPGA_HEAD_BUS_SPI															0
#define SE_FPGA_HEAD_BUS_QSPI														1

#define SE_FPGA_PACKAGE_DATA_MAX_LENGTH								SE_FPGA_PACKAGE_LENGTH_MASK

typedef union
{
	struct
	{
		SeUInt8 :3;
		SeUInt8 BusKind:1;
		SeUInt8 :3;
		SeUInt8 Rw:1;
	}Bit;
	SeUInt8 Byte;
}SeFpgaPackageHead;

typedef struct
{
	SeFpgaPackageHead tHead;
	SeUInt8 hPageAddress;
	SeUInt8 hRegisterAddress;
	SeUInt8 hDummy;
	SeUInt8* pData;
	SeUInt8 nLength;
}SeFpgaPackage;

static SeFpgaDescription tFpgaDesc;

static SeFpgaPackage SeFpgaPackageCombin(SeBool blIsRead, SeUInt16 hAddress, SeUInt8* pData, SeUInt8 nLength)
{
	SeFpgaPackage tPackage;
	memset(&tPackage, 0, sizeof(SeFpgaPackage));

	if(pData == SeNull)
	{
		return tPackage;
	}

	if(blIsRead == SeTrue)
	{
		tPackage.tHead.Bit.Rw = SE_FPGA_HEAD_READ_FLAG;
	}else{
		tPackage.tHead.Bit.Rw = SE_FPGA_HEAD_WRITE_FLAG;
	}

	tPackage.tHead.Bit.BusKind = SE_FPGA_HEAD_BUS_QSPI;

	tPackage.hPageAddress = (hAddress & 0xff00) >> 8;
	tPackage.hRegisterAddress = hAddress & 0xff;
	tPackage.pData = pData;
	tPackage.nLength = nLength;

	return tPackage;
}

 static SeInt8 SeFpgaPackageRead(SeFpgaPackage* pPackage)
{
    SeUInt8 iCount;

	SeGpioSetBit(tFpgaDesc.iFpgaCsPinIndex, SE_BIT_LOW);

	SeSpiWrite(tFpgaDesc.iFpgaInfoSpiIndex, pPackage->tHead);
	SeSpiWrite(tFpgaDesc.iFpgaInfoSpiIndex, pPackage->hPageAddress);
	SeSpiWrite(tFpgaDesc.iFpgaInfoSpiIndex, pPackage->hRegisterAddress);
	SeSpiWrite(tFpgaDesc.iFpgaInfoSpiIndex, pPackage->hDummy);

    for(iCount = 0; iCount < (pPackage->nLength); iCount++)
    {
    	if(SeSpiRead(tFpgaDesc.iFpgaDataSpiIndex, &(pPackage->pData[iCount])) == SE_RETURN_ERROR)
			return SE_RETURN_ERROR;
    }

	SeGpioSetBit(tFpgaDesc.iFpgaCsPinIndex, SE_BIT_HIGH);

	return SE_RETURN_OK;
}

static SeInt8 SeFpgaPackageWrite(SeFpgaPackage* pPackage)
{
    SeUInt8 iCount;

    SeGpioSetBit(tFpgaDesc.iFpgaCsPinIndex, SE_BIT_LOW);

    SeSpiWrite(tFpgaDesc.iFpgaInfoSpiIndex, pPackage->tHead);
    SeSpiWrite(tFpgaDesc.iFpgaInfoSpiIndex, pPackage->hPageAddress);
    SeSpiWrite(tFpgaDesc.iFpgaInfoSpiIndex, pPackage->hRegisterAddress);
	//SeSpiWrite(tFpgaDesc.iFpgaInfoSpiIndex, pPackage->hDummy);

    for(iCount = 0; iCount < (pPackage->nLength); iCount++)
	{
    	if(SeSpiWrite(tFpgaDesc.iFpgaDataSpiIndex, pPackage->pData[iCount]) == SE_RETURN_ERROR)
    		return SE_RETURN_ERROR;
	}

    SeGpioSetBit(tFpgaDesc.iFpgaCsPinIndex, SE_BIT_HIGH);
	return SE_RETURN_OK;
}

SeInt8 SeFpgaInit(SeFpgaDescription tFpgaDescription)
{
	tFpgaDesc = tFpgaDescription;
	if(SeGpioSetBit(tFpgaDesc.iFpgaCsPinIndex, SE_BIT_HIGH) != SE_RETURN_OK)
	{
		SeErrorPrint("Disable FPGA fail!");
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeFpgaReadDatas(SeUInt16 hAddress, SeUInt8* pData, SeUInt8 nLength)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeTrue, hAddress, pData, nLength);

	if(SeFpgaPackageRead(&tPackage) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}
SeInt8 SeFpgaWriteDatas(SeUInt16 hAddress, SeUInt8* pData, SeUInt8 nLength)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeFalse, hAddress, pData, nLength);

	if(SeFpgaPackageWrite(&tPackage) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeFpgaRead8BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType8Bit* pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeTrue, hAddress, pData, 1);

	return SeFpgaPackageRead(&tPackage);
}

SeInt8 SeFpgaRead16BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType16Bit* pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeTrue, hAddress, pData->aData, 2);

	return SeFpgaPackageRead(&tPackage);
}

SeInt8 SeFpgaRead24BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType24Bit* pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeTrue, hAddress, pData->aData, 3);
	return SeFpgaPackageRead(&tPackage);
}

SeInt8 SeFpgaRead32BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType32Bit* pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeTrue, hAddress, pData->aData, 4);
	return SeFpgaPackageRead(&tPackage);
}

SeInt8 SeFpgaRead48BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType48Bit* pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeTrue, hAddress, pData->aData, 6);
	return SeFpgaPackageRead(&tPackage);
}

SeInt8 SeFpgaRead64BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType64Bit* pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeTrue, hAddress, pData->aData, 8);
	return SeFpgaPackageRead(&tPackage);
}

SeInt8 SeFpgaWrite8BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType8Bit pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeFalse, hAddress, &pData, 1);
	return SeFpgaPackageWrite(&tPackage);
}

SeInt8 SeFpgaWrite16BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType16Bit pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeFalse, hAddress, pData.aData, 2);
	return SeFpgaPackageWrite(&tPackage);
}

SeInt8 SeFpgaWrite24BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType24Bit pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeFalse, hAddress, pData.aData, 3);
	return SeFpgaPackageWrite(&tPackage);
}

SeInt8 SeFpgaWrite32BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType32Bit pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeFalse, hAddress, pData.aData, 4);
	return SeFpgaPackageWrite(&tPackage);
}

SeInt8 SeFpgaWrite48BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType48Bit pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeFalse, hAddress, pData.aData, 6);
	return SeFpgaPackageWrite(&tPackage);
}

SeInt8 SeFpgaWrite64BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType64Bit pData)
{
	SeFpgaPackage tPackage;

	tPackage = SeFpgaPackageCombin(SeFalse, hAddress, pData.aData, 8);
	return SeFpgaPackageWrite(&tPackage);
}
