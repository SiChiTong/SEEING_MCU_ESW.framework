/*
 * Convert.h
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#ifndef _CONVERT_H_
#define _CONVERT_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

SeInt16 SeStringToInteger(SeString sDecimal);

SeUInt32 SeStringToHex(SeString sHexadecimal);

SeUInt8 SeStringToUInt8(SeString sDecimal);
SeUInt16 SeStringToUInt16(SeString sDecimal);
SeUInt32 SeStringToUInt32(SeString sDecimal);
SeUInt64 SeStringToUInt64(SeString sDecimal);

SeString SeUtcTimeToString(SeTimeValue tUtcTime);
SeTimeValue SeStringToUtcTime(SeString sDateAndTime);

#ifdef __cplusplus
}
#endif

#endif /* CONVERT_H_ */
