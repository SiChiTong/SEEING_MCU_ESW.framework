/*
 * Internal.c
 *
 *  Created on: 2015-11-16
 *      Author: Administrator
 */


#include "Internal.h"


SeInt8 SeInternalSpiInit(void* pKindOperation)
{
	SeInternalSpiOperation* pOper = (SeInternalSpiOperation*)pKindOperation;

	if(pOper == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeInternalSpiPreInit() == SE_RETURN_ERROR)
	{

		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeInternalSpiRead(void* pKindOperation, SeUInt8* bData)
{
	SeBool blResult = SeTrue;
	SeInternalSpiOperation* pOper = (SeInternalSpiOperation*)pKindOperation;

	if(pOper == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(bData == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	pOper->fpSeInternalSpiClrCs();

	if(pOper->fpSeInternalSpiRead(bData) == SE_RETURN_ERROR)
	{
		blResult = SeFalse;
	}

	pOper->fpSeInternalSpiSetCs();

	if(blResult == SeFalse)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeInternalSpiWrite(void* pKindOperation, SeUInt8 bData)
{
	SeBool blResult = SeTrue;
	SeInternalSpiOperation* pOper = (SeInternalSpiOperation*)pKindOperation;

	if(pOper == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	pOper->fpSeInternalSpiClrCs();

	if(pOper->fpSeInternalSpiWrite(bData) == SE_RETURN_ERROR)
	{
		blResult = SeFalse;
	}

	pOper->fpSeInternalSpiSetCs();

	if(blResult == SeFalse)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}
