#include "Include.h"
#include <map>

typedef union {
	SeFpgaSpiRegisterType8Bit reg8Bit;
	SeFpgaSpiRegisterType16Bit reg16Bit;
	SeFpgaSpiRegisterType24Bit reg24Bit;
	SeFpgaSpiRegisterType32Bit reg32Bit;
	SeFpgaSpiRegisterType48Bit reg48Bit;
	SeFpgaSpiRegisterType64Bit reg64Bit;
}SeMockFpgaData;

std::map<SeUInt16, SeMockFpgaData> gMockFpgaMap;

SeInt8 SeFpgaReadDatas(SeUInt16 hAddress, SeUInt8* pData, SeUInt8 nLength)
{
	SeUInt8 iIndex;
	for(iIndex = 0; iIndex < nLength; iIndex++)
	{
		pData[iIndex] = gMockFpgaMap[hAddress].reg64Bit.aData[iIndex];
	}

	return SE_RETURN_OK;
}

SeInt8 SeFpgaWriteDatas(SeUInt16 hAddress, SeUInt8* pData, SeUInt8 nLength)
{
	SeUInt8 iIndex;
	for(iIndex = 0; iIndex < nLength; iIndex++)
	{
		gMockFpgaMap[hAddress].reg64Bit.aData[iIndex] = pData[iIndex];
	}

	return SE_RETURN_OK;
}

SeInt8 SeFpgaRead8BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType8Bit* pData)
{
	*pData = gMockFpgaMap[hAddress].reg8Bit;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaRead16BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType16Bit* pData)
{
	pData->hData = gMockFpgaMap[hAddress].reg16Bit.hData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaRead24BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType24Bit* pData)
{
	pData->hData = gMockFpgaMap[hAddress].reg24Bit.hData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaRead32BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType32Bit* pData)
{
	pData->hData = gMockFpgaMap[hAddress].reg32Bit.hData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaRead48BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType48Bit* pData)
{
	pData->hData = gMockFpgaMap[hAddress].reg48Bit.hData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaRead64BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType64Bit* pData)
{
	pData->hData = gMockFpgaMap[hAddress].reg64Bit.hData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaWrite8BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType8Bit pData)
{
	gMockFpgaMap[hAddress].reg8Bit = pData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaWrite16BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType16Bit pData)
{
	gMockFpgaMap[hAddress].reg16Bit.hData = pData.hData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaWrite24BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType24Bit pData)
{
	gMockFpgaMap[hAddress].reg24Bit.hData = pData.hData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaWrite32BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType32Bit pData)
{
	gMockFpgaMap[hAddress].reg32Bit.hData = pData.hData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaWrite48BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType48Bit pData)
{
	gMockFpgaMap[hAddress].reg48Bit.hData = pData.hData;
	return SE_RETURN_OK;
}

SeInt8 SeFpgaWrite64BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType64Bit pData)
{
	gMockFpgaMap[hAddress].reg64Bit.hData = pData.hData;
	return SE_RETURN_OK;
}

