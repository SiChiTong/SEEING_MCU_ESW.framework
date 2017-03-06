/*
 * Simulate.c
 *
 *  Created on: 2015-11-16
 *      Author: Administrator
 */


#include "Simulate.h"


SeInt8 SeSimulateSpiInit(void* pKindOperation)
{
	SeSimulateSpiOperation* pOper = (SeSimulateSpiOperation*)pKindOperation;

	if(pOper == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeSimulateSpiPreInit() == SE_RETURN_ERROR)
	{

		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeSimulateSpiRead(void* pKindOperation, SeUInt8* bData)
{
	SeInt8 iCursor;
	SeUInt8 bTemp = 0;
	SeSimulateSpiOperation* pOper = (SeSimulateSpiOperation*)pKindOperation;

	if(pOper == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(bData == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	pOper->fpSeSimulateSpiClrCs();

	for(iCursor = 0; iCursor < 8; iCursor++)
	{
		bTemp = (bTemp << 1);
		pOper->fpSeSimulateSpiClrScl();
		pOper->fpSeSimulateSpiSetScl();
		bTemp |= pOper->fpSeSimulateSpiGetSdi();
	}

	pOper->fpSeSimulateSpiSetCs();

	*bData = bTemp;

	return SE_RETURN_OK;
}

SeInt8 SeSimulateSpiWrite(void* pKindOperation, SeUInt8 bData)
{
	SeInt8 iCursor;
	SeUInt8 bTemp = bData;
	SeSimulateSpiOperation* pOper = (SeSimulateSpiOperation*)pKindOperation;

	if(pOper == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	pOper->fpSeSimulateSpiClrCs();

	for(iCursor = 0; iCursor < 8; iCursor++)
	{
		pOper->fpSeSimulateSpiClrScl();
		if((bTemp & 0x80) == 0x80)
		{
			pOper->fpSeSimulateSpiSetSdo();
		}else{
			pOper->fpSeSimulateSpiClrSdo();
		}
		pOper->fpSeSimulateSpiSetScl();
		bTemp = (bTemp << 1);
	}

	pOper->fpSeSimulateSpiSetCs();

	return SE_RETURN_OK;
}
