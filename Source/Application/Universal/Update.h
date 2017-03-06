/*
 * Update.h
 *
 *  Created on: 2016年2月29日
 *      Author: seeing
 */

#ifndef SOURCE_APPLICATION_UNIVERSAL_UPDATE_H_
#define SOURCE_APPLICATION_UNIVERSAL_UPDATE_H_

#include "../../Global/Include.h"

typedef SeInt8 (*SeUpdateCallback)(void);

typedef struct
{
	SeUpdateCallback fpSeUpdateMcu;
	SeUpdateCallback fpSeUpdateFpga;
}SeUpdateDescription;

void SeUpdateConfig(SeUpdateDescription tUpdateDescription);
SeInt8 SeUpdateMcuXmodem();
SeInt8 SeUpdateFpgaXmodem();

void SeTftpUpdateOperationSet(SeTftpClientGetFileOperation tTftpClientGetFileOperation);
SeTftpClientGetFileOperation SeTftpUpdateOperationGet(void);

#endif /* SOURCE_APPLICATION_UNIVERSAL_UPDATE_H_ */
