/*
 * Uart1.h
 *
 *  Created on: 2015-12-14
 *      Author: Administrator
 */

#ifndef _STM32F107_UART1_H_
#define _STM32F107_UART1_H_

#include "../../../../Global/Include.h"

SeInt8 SeStm32f107Uart1Init(SeUInt32 iBaudRate);
SeInt8 SeStm32f107Uart1ReadByte(SeUInt8* bData);
SeInt8 SeStm32f107Uart1WriteByte(SeUInt8 bData);

#endif /* _STM32F107_UART1_H_ */
