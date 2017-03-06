/*
 * Cat9555.h
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */

#ifndef _CAT9555_H_
#define _CAT9555_H_

#include "../../../Global/Include.h"


/*
 * Cat9555 interface function
 */
SeInt8 SeCat9555Init(SeCat9555Description* pCat9555Description);
SeInt8 SeCat9555InputOutputConfig(SeCat9555Description* pCat9555Description, SeUInt16 iSetData);
SeInt8 SeCat9555ReadInputPortData(SeCat9555Description* pCat9555Description, SeUInt16 *pData);
SeInt8 SeCat9555WriteOutPortData(SeCat9555Description* pCat9555Description, SeUInt16 iData);
SeInt8 SeCat9555ReadOutPortRegistorValue(SeCat9555Description* pCat9555Description, SeUInt16 *pData);
SeInt8 SeCat9555WriteInputPortInversion(SeCat9555Description* pCat9555Description, SeUInt16 iData);
SeInt8 SeCat9555ReadInputPinNumStatus(SeCat9555Description* pCat9555Description, SeUInt8 iPinIndex, SeUInt8 *pStatus);
SeInt8 SeCat9555WriteOutPinNumStatus(SeCat9555Description* pCat9555Description, SeUInt8 iPinIndex, SeUInt8 iStatus);
SeInt8 SeCat9555PinsSetLow(SeCat9555Description* pCat9555Description, SeUInt16 iPinx);
SeInt8 SeCat9555PinsSetHigh(SeCat9555Description* pCat9555Description, SeUInt16 iPinx);

#endif 
