/*
 * Uart.c
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#include "Uart.h"

static SeUartDescription* pUartPorts[CONFIG_MAX_UART_PORTS] = {0};

SeInt8 SeUartInit(SeInt8 iIndex, SeUartDescription* pUart)
{
	SeUartOperation* pOper = SeNull;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_UART_PORTS - 1))
	{

		return SE_RETURN_ERROR;
	}
	if(pUart == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	if(pUart->pOperation == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	pOper = (SeUartOperation*)(pUart->pOperation);

	if(pOper->fpSeUartPreInit(pUart->iBaudRate) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	pUartPorts[iIndex] = pUart;
	pUartPorts[iIndex]->blInited = SeTrue;

	return SE_RETURN_OK;
}

SeInt8 SeUartRead(SeInt8 iIndex, SeUInt8* pData, SeInt16 nRead, SeInt16* nReaded)
{
	SeUartOperation* pOper = SeNull;

	if(iIndex < 0 || iIndex > (CONFIG_MAX_UART_PORTS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pData == SeNull || nReaded == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(pUartPorts[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pUartPorts[iIndex]->blInited == SeFalse)
	{

		return SE_RETURN_ERROR;
	}

	pOper = (SeUartOperation*)(pUartPorts[iIndex]->pOperation);

	while(*nReaded != nRead)
	{
		if(pOper->fpSeUartReadByte(pData + *nReaded) == SE_RETURN_ERROR)
			break;
		(*nReaded)++;
	}

	return SE_RETURN_OK;
}

SeInt8 SeUartWrite(SeInt8 iIndex, SeUInt8* pData, SeInt16 nWrite, SeInt16* nWrited)
{
	SeUartOperation* pOper = SeNull;

	if(iIndex < 0 || iIndex > (CONFIG_MAX_UART_PORTS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pData == SeNull || nWrited == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(pUartPorts[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pUartPorts[iIndex]->blInited == SeFalse)
	{

		return SE_RETURN_ERROR;
	}

	pOper = (SeUartOperation*)(pUartPorts[iIndex]->pOperation);

	while(*nWrited != nWrite)
	{
		pOper->fpSeUartWriteByte(*(pData + *nWrited));
		(*nWrited)++;
	}

	return SE_RETURN_OK;
}
