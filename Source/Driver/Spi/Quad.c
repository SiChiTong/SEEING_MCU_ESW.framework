/*
 * Quad.c
 *
 *  Created on: 2015-11-16
 *      Author: Administrator
 */

#include "Quad.h"

SeInt8 SeQuadSpiInit(void* pKindOperation)
{
	SeQuadSpiOperation* pOper = (SeQuadSpiOperation*)pKindOperation;

	if(pOper == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(pOper->fpSeQuadSpiPreInit() == SE_RETURN_ERROR)
	{

		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeQuadSpiRead(void* pKindOperation, SeUInt8* bData)
{
	SeInt8 iCursor;
	SeUInt8 bTemp = 0;
	SeQuadSpiOperation* pOper = (SeQuadSpiOperation*)pKindOperation;

	if(pOper == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(bData == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	pOper->fpSeQuadSpiClrCs();

	for(iCursor = 0; iCursor < 2; iCursor++)
	{
		SeQuadSpiData tSpiData;
		bTemp = (bTemp << 4);
		pOper->fpSeQuadSpiClrScl();
		pOper->fpSeQuadSpiSetScl();
		tSpiData = pOper->fpSeQuadSpiGetSdi();
		bTemp |= tSpiData.Byte;
	}

	pOper->fpSeQuadSpiSetCs();

	*bData = bTemp;

	return SE_RETURN_OK;
}

SeInt8 SeQuadSpiWrite(void* pKindOperation, SeUInt8 bData)
{
	SeUInt8 bTemp = bData;
	SeQuadSpiData tSpiData;
	SeQuadSpiOperation* pOper = (SeQuadSpiOperation*)pKindOperation;

	if(pOper == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	pOper->fpSeQuadSpiClrCs();

	/*
	 * musk low bits
	 */
	tSpiData.Byte = ((bTemp & 0xF0) >> 4);
	pOper->fpSeQuadSpiClrScl();
	pOper->fpSeQuadSpiSetSdo(tSpiData);
	pOper->fpSeQuadSpiSetScl();

	/*
	 * musk high bits
	 */
	tSpiData.Byte = (bTemp & 0x0F);
	pOper->fpSeQuadSpiClrScl();
	pOper->fpSeQuadSpiSetSdo(tSpiData);
	pOper->fpSeQuadSpiSetScl();

	pOper->fpSeQuadSpiSetCs();

	return SE_RETURN_OK;
}
