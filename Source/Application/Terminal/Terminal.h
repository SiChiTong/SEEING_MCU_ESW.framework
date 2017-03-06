/*
 * Terminal.h
 *
 *  Created on: 2015-11-28
 *      Author: Administrator
 */

#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

SeInt8 SeTerminalInit(void);

void SeTerminalWelcome(void);

SeInt8 SeTerminalProcess(void);

#ifdef __cplusplus
}
#endif

#endif /* _TERMINAL_H_ */
