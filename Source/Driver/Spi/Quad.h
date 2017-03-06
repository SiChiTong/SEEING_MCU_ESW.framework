/*
 * Quad.h
 *
 *  Created on: 2015-11-16
 *      Author: Administrator
 */

#ifndef _QUAD_SPI_H_
#define _QUAD_SPI_H_

#include "../../Global/Include.h"

SeInt8 SeQuadSpiInit(void* pKindOperation);
SeInt8 SeQuadSpiRead(void* pKindOperation, SeUInt8* bData);
SeInt8 SeQuadSpiWrite(void* pKindOperation, SeUInt8 bData);

#endif /* _QUAD_SPI_H_ */
