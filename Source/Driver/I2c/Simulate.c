/*
 * Simulate.c
 *
 *  Created on: 2015-12-2
 *      Author: Administrator
 */


#include "Simulate.h"

SeInt8 SeSimulateI2cInit(void* pKindOperation)
{
	SeSimulateI2cOperation* pOper = (SeSimulateI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeSimulateI2cPreInit() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeSimulateI2cStart(void* pKindOperation)
{
	SeSimulateI2cOperation* pOper = (SeSimulateI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pOper->fpSeSimulateI2cSetSda();
	pOper->fpSeSimulateI2cSetScl();
	if(pOper->fpSeSimulateI2cGetSda() == SE_BIT_LOW)
	{
		return SE_RETURN_ERROR;
	}

	pOper->fpSeSimulateI2cClrSda();
	pOper->fpSeSimulateI2cClrScl();
	if(pOper->fpSeSimulateI2cGetSda() == SE_BIT_HIGH)
	{
		return SE_RETURN_ERROR;
	}


	return SE_RETURN_OK;
}

SeInt8 SeSimulateI2cStop(void* pKindOperation)
{
	SeSimulateI2cOperation* pOper = (SeSimulateI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pOper->fpSeSimulateI2cClrScl();
	pOper->fpSeSimulateI2cClrSda();
	pOper->fpSeSimulateI2cSetScl();
	pOper->fpSeSimulateI2cSetSda();

	return SE_RETURN_OK;
}

SeInt8 SeSimulateI2cAck(void* pKindOperation)
{
	SeSimulateI2cOperation* pOper = (SeSimulateI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pOper->fpSeSimulateI2cClrSda();
	pOper->fpSeSimulateI2cSetScl();
	pOper->fpSeSimulateI2cClrScl();

	return SE_RETURN_OK;
}

SeInt8 SeSimulateI2cNack(void* pKindOperation)
{
	SeSimulateI2cOperation* pOper = (SeSimulateI2cOperation*)pKindOperation;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pOper->fpSeSimulateI2cSetSda();
	pOper->fpSeSimulateI2cSetScl();
	pOper->fpSeSimulateI2cClrScl();

	return SE_RETURN_OK;
}

SeInt8 SeSimulateI2cRead(void* pKindOperation, SeUInt8* bData, SeBool blAck)
{
	SeSimulateI2cOperation* pOper = (SeSimulateI2cOperation*)pKindOperation;
	SeInt8 nCount;
	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(bData == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	*bData = 0;

	pOper->fpSeSimulateI2cSetSda();
	pOper->fpSeSimulateI2cClrScl();

	for(nCount = 0; nCount < 8; nCount++)
	{
		*bData <<= 1;
		pOper->fpSeSimulateI2cSetScl();
		if(pOper->fpSeSimulateI2cGetSda() == SE_BIT_HIGH)
		{
			*bData |= 0x01;
		}
		pOper->fpSeSimulateI2cClrScl();
	}

	/*
	 * send ack
	 */
	if(blAck == SeTrue)
	{
		pOper->fpSeSimulateI2cSetSda();
		pOper->fpSeSimulateI2cSetScl();
		pOper->fpSeSimulateI2cClrScl();
	}

	return SE_RETURN_OK;
}

SeInt8 SeSimulateI2cWrite(void* pKindOperation, SeUInt8 bData, SeBool* blAck)
{
	SeSimulateI2cOperation* pOper = (SeSimulateI2cOperation*)pKindOperation;
	SeInt8 nCount;
	SeUInt8 bTempData = bData;

	if(pOper == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(blAck == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pOper->fpSeSimulateI2cClrScl();
	for(nCount = 0; nCount < 8; ++nCount)
	{
		if(bTempData & 0x80)
		{
			pOper->fpSeSimulateI2cSetSda();
		}else{
			pOper->fpSeSimulateI2cClrSda();
		}
		bTempData <<= 1;
		pOper->fpSeSimulateI2cSetScl();
		pOper->fpSeSimulateI2cClrScl();
	}

	/*
	 * get ack status
	 */
	pOper->fpSeSimulateI2cSetSda();
	pOper->fpSeSimulateI2cSetScl();
	*blAck = SeTrue;
#if 0
	if(pOper->fpSeSimulateI2cGetSda() == SE_BIT_HIGH)
	{
		*blAck = SeFalse;
	}else{
		*blAck = SeTrue;
	}
#endif
	pOper->fpSeSimulateI2cClrScl();

	return SE_RETURN_OK;
}
