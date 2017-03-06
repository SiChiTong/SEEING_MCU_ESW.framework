/*
 * Handle.h
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#ifndef _HANDLE_H_
#define _HANDLE_H_

#include "../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

SeInt8 SeHelpHandle(SeLinkList* pParameterList, SeString sResult);
//void SeHelpHandle();

SeInt8 SeFpgaReadHandle(SeLinkList* pParameterList, SeString sResult);
SeInt8 SeFpgaWriteHandle(SeLinkList* pParameterList, SeString sResult);
SeInt8 SeFpgaThdnMeasure(SeLinkList* pParameterList, SeString sResult);
SeInt8 SeFpgaPulseWidthMeasureHandle(SeLinkList* pParameterList, SeString sResult);
SeInt8 SeFpgaCodecClosePdmHandle(SeLinkList* pParameterList, SeString sResult);
SeInt8 SeFpgaCodecOpenPdmHandle(SeLinkList* pParameterList, SeString sResult);
SeInt8 SeFpgaAmplitudeMeasureHandle(SeLinkList* pParameterList, SeString sResult);
SeInt8 SeFpgaFrequencyMeasureHandle(SeLinkList* pParameterList, SeString sResult);
SeInt8 SeFpgaUartConfigHandle(SeLinkList* pParameterList, SeString sResult);

SeInt8 SeAd9265DataAcquisitionHandle(SeLinkList* pParameterList, SeString sResult);

SeInt8 SeUpdateHandle(SeLinkList* pParameterList, SeString sResult);
SeInt8 SeTftpUpdateHandle(SeLinkList* pParameterList, SeString sResult);

#ifdef __cplusplus
}
#endif

#endif

