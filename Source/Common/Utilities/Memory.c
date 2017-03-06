/*
 * Memory.c
 *
 *  Created on: 2015-11-30
 *      Author: Administrator
 */


#include "Memory.h"

void SeMemoryInit(void)
{

}

void* SeMemoryAlloc(SeUInt32 nSize)
{
#ifdef CONFIG_USING_FREERTOS
	return pvPortMalloc(nSize);
#endif
}

void SeMemoryFree(void* pAddress)
{
#ifdef CONFIG_USING_FREERTOS
	vPortFree(pAddress);
#endif
}
