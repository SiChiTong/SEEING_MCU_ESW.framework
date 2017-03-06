/*
 * Uart.h
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#ifndef _UART_H_
#define _UART_H_

#include "../../Global/Include.h"

SeInt8 SeUartInit(SeInt8 iIndex, SeUartDescription* pUart);
SeInt8 SeUartRead(SeInt8 iIndex, SeUInt8* pData, SeInt16 nRead, SeInt16* nReaded);
SeInt8 SeUartWrite(SeInt8 iIndex, SeUInt8* pData, SeInt16 nWrite, SeInt16* nWrited);

#endif /* _UART_H_ */
