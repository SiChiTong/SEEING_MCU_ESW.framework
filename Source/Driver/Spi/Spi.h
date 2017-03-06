

#ifndef _SPI_H_
#define _SPI_H_

#include "../../Global/Include.h"

SeInt8 SeSpiInit(SeInt8 iIndex, SeSpiBusDescription* pSpi);
SeInt8 SeSpiRead(SeInt8 iIndex, SeUInt8* bData);
SeInt8 SeSpiWrite(SeInt8 iIndex, SeUInt8 bData);

#endif
