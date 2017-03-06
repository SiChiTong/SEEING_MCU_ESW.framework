/*
 * Arithmetic.h
 *
 *  Created on: 2016年1月7日
 *      Author: seeing
 */

#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

SeInt8 SeLargeNumberAdd(SeUInt8 *pX, SeUInt8 nSizeX, SeUInt8 *pY, SeUInt8 nSizeY);

SeInt8 SeLargeNumberSub(SeUInt8 *pX, SeUInt8 nSizeX, SeUInt8 *pY, SeUInt8 nSizeY);

SeInt8 SeLargeNumberMul(SeUInt8 *pX,SeUInt8 nSizeX, SeUInt8 *pY, SeUInt8 nSizeY);

SeInt8 SeLargeNumberDiv(SeUInt8 *pX, SeUInt8 nSizeX, SeUInt8 *pY, SeUInt8 nSizeY, SeUInt8 *pQ, SeUInt8 *nSizeQ);


SeUInt16 SeCrc16Ccitt(SeInt8* pData, SeUInt16 nLength);

#ifdef __cplusplus
}
#endif

#endif /* ARITHMETIC_H_ */
