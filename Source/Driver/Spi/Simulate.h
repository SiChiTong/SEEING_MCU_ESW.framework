/*
 * Simulate.h
 *
 *  Created on: 2015-11-16
 *      Author: Administrator
 */

#ifndef _SIMULATE_SPI_H_
#define _SIMULATE_SPI_H_

#include "../../Global/Include.h"

SeInt8 SeSimulateSpiInit(void* pKindOperation);
SeInt8 SeSimulateSpiRead(void* pKindOperation, SeUInt8* bData);
SeInt8 SeSimulateSpiWrite(void* pKindOperation, SeUInt8 bData);

#endif /* _SIMULATE_SPI_H_ */
