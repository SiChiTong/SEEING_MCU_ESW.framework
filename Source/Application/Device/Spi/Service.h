/*
 * Register.h
 *
 *  Created on: 2017年2月23日
 *      Author: seeing
 */

#ifndef _DEVICE_SPI_SERVICE_H_
#define _DEVICE_SPI_SERVICE_H_

#include "../../../Global/Include.h"

typedef void (*SeSpiSlaveServiceHandle)(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nSize);

SeInt8 SeSpiSlaveServiceInit(SeInt8 iSpiSlaveIndex, SeSpiSlaveDescription tSpiSlave, SeInt16 nServiceSpaceSize);
SeInt8 SeSpiSlaveServiceRegisterHandle(SeUInt16 hAddress, SeSpiSlaveServiceHandle cbServiceHandle);

void SeSpiSlaveSetRegister(SeUInt16 hAddress, SeUInt8* pData, SeUInt16 nSize);
void SeSpiSlaveGetRegister(SeUInt16 hAddress, SeUInt8* pData, SeUInt16 nSize);

#endif /* _DEVICE_SPI_SERVICE_H_ */
