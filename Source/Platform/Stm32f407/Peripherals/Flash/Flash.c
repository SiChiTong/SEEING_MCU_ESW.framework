/*
 * Flash.c
 *
 *  Created on: 2016年2月22日
 *      Author: seeing
 */

#include "Flash.h"

SeInt8 SeStm32f407FlashWritedata(SeUInt32 nAddress, SeUInt8 * pData, SeUInt32 nLength)
{
	SeUInt32 nEndAddress = nAddress + nLength;

	if(nEndAddress > USER_FLASH_END_ADDRESS)
	{
		SeErrorPrint("write address more than the end address");
		return SE_RETURN_ERROR;
	}

	while (nAddress < nEndAddress)
	{
	    if (FLASH_ProgramByte(nAddress, *pData++) == FLASH_COMPLETE)
	    	nAddress++;
	    else
	    {
	    	SeErrorPrint("write stm32f407 flash error");
	    	return SE_RETURN_ERROR;
	    }
	}

	return SE_RETURN_OK;
}

