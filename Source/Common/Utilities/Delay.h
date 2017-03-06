/*
 * Delay.h
 *
 *  Created on: 2015-11-19
 *      Author: Administrator
 */

#ifndef _DELAY_H_
#define _DELAY_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * systick or RTC config for delay method
 */
SeInt8 SeDelayInit(SeDelayOperation tDelayOper);

/*
 * delay Milliseconds
 */
void SeDelayMs(SeUInt32 nMs);

/*
 * delay for some non-operation instructs
 */
void SeDelayCycles(SeUInt32 nCycles);

#ifdef __cplusplus
}
#endif

#endif /* _DELAY_H_ */
