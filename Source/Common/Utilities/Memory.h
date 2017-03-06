/*
 * Memory.h
 *
 *  Created on: 2015-11-30
 *      Author: Administrator
 */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

void SeMemoryInit(void);
void* SeMemoryAlloc(SeUInt32 nSize);
void SeMemoryFree(void* pAddress);

#ifdef __cplusplus
}
#endif

#endif /* _MEMORY_H_ */
