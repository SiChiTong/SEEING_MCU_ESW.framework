/*
 * Internal.h
 *
 *  Created on: 2015-11-16
 *      Author: Administrator
 */

#ifndef _INTERNAL_SPI_H_
#define _INTERNAL_SPI_H_

#include "../../Global/Include.h"

SeInt8 SeInternalSpiInit(void* pKindOperation);
SeInt8 SeInternalSpiRead(void* pKindOperation, SeUInt8* bData);
SeInt8 SeInternalSpiWrite(void* pKindOperation, SeUInt8 bData);

#endif /* _INTERNAL_SPI_H_ */
