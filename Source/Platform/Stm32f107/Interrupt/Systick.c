/*
 * SystickIsr.c
 *
 *  Created on: 2016年2月2日
 *      Author: seeing
 */

#include "../../System/System.h"

extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
	xPortSysTickHandler();
}

