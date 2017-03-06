/*
 * Internal.c
 *
 *  Created on: 2015-12-2
 *      Author: Administrator
 */


#include "Internal.h"

SeInt8 SeInternalI2cInit(void* pKindOperation)
{
	SeInternalI2cOperation* pOper = (SeInternalI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeInternalI2cPreInit() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeInternalI2cStart(void* pKindOperation)
{
	SeInternalI2cOperation* pOper = (SeInternalI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeInternalI2cStart() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeInternalI2cStop(void* pKindOperation)
{
	SeInternalI2cOperation* pOper = (SeInternalI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeInternalI2cStop() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeInternalI2cAck(void* pKindOperation)
{
	SeInternalI2cOperation* pOper = (SeInternalI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeInternalI2cAck() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeInternalI2cNack(void* pKindOperation)
{
	SeInternalI2cOperation* pOper = (SeInternalI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeInternalI2cNack() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeInternalI2cRead(void* pKindOperation, SeUInt8* bData, SeBool blAck)
{
	SeInternalI2cOperation* pOper = (SeInternalI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeInternalI2cRead(bData, blAck) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeInternalI2cWrite(void* pKindOperation, SeUInt8 bData, SeBool* blAck)
{
	SeInternalI2cOperation* pOper = (SeInternalI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeInternalI2cWrite(bData, blAck) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

