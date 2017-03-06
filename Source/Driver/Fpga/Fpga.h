/*
 * Fpga.h
 *
 *  Created on: 2016年1月5日
 *      Author: seeing
 */

#ifndef _FPGA_H_
#define _FPGA_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	SeInt8 iFpgaInfoSpiIndex;
	SeInt8 iFpgaDataSpiIndex;
	SeInt8 iFpgaCsPinIndex;
}SeFpgaDescription;

SeInt8 SeFpgaInit(SeFpgaDescription tFpgaDescription);

SeInt8 SeFpgaReadDatas(SeUInt16 hAddress, SeUInt8* pData, SeUInt8 nLength);
SeInt8 SeFpgaWriteDatas(SeUInt16 hAddress, SeUInt8* pData, SeUInt8 nLength);

SeInt8 SeFpgaRead8BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType8Bit* pData);
SeInt8 SeFpgaRead16BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType16Bit* pData);
SeInt8 SeFpgaRead24BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType24Bit* pData);
SeInt8 SeFpgaRead32BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType32Bit* pData);
SeInt8 SeFpgaRead48BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType48Bit* pData);
SeInt8 SeFpgaRead64BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType64Bit* pData);

SeInt8 SeFpgaWrite8BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType8Bit pData);
SeInt8 SeFpgaWrite16BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType16Bit pData);
SeInt8 SeFpgaWrite24BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType24Bit pData);
SeInt8 SeFpgaWrite32BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType32Bit pData);
SeInt8 SeFpgaWrite48BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType48Bit pData);
SeInt8 SeFpgaWrite64BitRegister(SeUInt16 hAddress, SeFpgaSpiRegisterType64Bit pData);

#ifdef __cplusplus
}
#endif

#endif /* _FPGA_H_ */
