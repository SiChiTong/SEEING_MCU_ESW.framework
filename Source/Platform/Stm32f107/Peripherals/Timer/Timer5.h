/*
 * Timer5.h
 *
 *  Created on: 2017年2月28日
 *      Author: seeing
 */

#ifndef _STM32F107_TIMER_TIMER5_H_
#define _STM32F107_TIMER_TIMER5_H_

#include "../../../../Global/Include.h"

SeInt8 SeStm32f107Timer5Init(SeTimeValue tInterval, SeTimerCallback tCallback);
void SeStm32f107Timer5Start(void);
void SeStm32f107Timer5Stop(void);

#endif /* SOURCE_PLATFORM_STM32F107_PERIPHERALS_TIMER_TIMER5_H_ */
