/*
 * Startup.c
 *
 *  Created on: 2015-12-18
 *      Author: Administrator
 */


#include "Include.h"

SeInt8 main(void)
{
	if(SeSystemInit() == SE_RETURN_OK)
	{
		SeNormalPrint("System Initial Success!");
	}else{
		SeNormalPrint("System Initial Failure!");
	}

	SeTaskSchedule();

	return 0;
}
