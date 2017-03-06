/*
 * Initial.c
 *
 *  Created on: 2015-12-18
 *      Author: Administrator
 */

#include "Initial.h"

SeInt8 SeSystemInit(void)
{
	if(SeUserDriverInit() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	if(SeUserApplicationInit() != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}
