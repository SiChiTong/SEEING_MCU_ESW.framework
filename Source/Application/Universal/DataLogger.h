/*
 * DataLogger.h
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#ifndef _DATALOGGER_APPLICATION_H_
#define _DATALOGGER_APPLICATION_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

SeInt8 SeNetworkAcquisitionAd9265Prepare(SeUInt16 nSampleTime);
SeInt8 SeNetworkAcquisitionAd9265GetDataSize(SeUInt32* nTotalSize);
SeInt8 SeNetworkAcquisitionAd9265ReadData(SeUInt32 iSeekPosition, SeUInt8* pData, SeUInt16 nBytesToRead, SeUInt16* nBytesReaded);
SeInt8 SeNetworkAcquisitionAd9265Finish(void);

#ifdef __cplusplus
}
#endif

#endif /* _DATALOGGER_APPLICATION_H_ */
