/*
 * I2c.c
 *
 *  Created on: 2015-12-1
 *      Author: Administrator
 */

#include "I2c.h"

static SeI2cBusDescription* pI2cBuses[CONFIG_MAX_I2C_PERIPHERALS] = {0};

SeInt8 SeI2cInit(SeInt8 iIndex, SeI2cBusDescription* pI2c)
{
	SeInt8 iResult;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_I2C_PERIPHERALS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pI2c == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pI2c->pOperation == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	switch(pI2c->iType)
	{
		case SE_I2C_BUS_TYPE_INTERNAL:
			iResult = SeInternalI2cInit(pI2c->pOperation);
			break;
		case SE_I2C_BUS_TYPE_SIMULATE:
			iResult = SeSimulateI2cInit(pI2c->pOperation);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	pI2cBuses[iIndex] = pI2c;

	pI2cBuses[iIndex]->blInited = SeTrue;


	return SE_RETURN_OK;
}

SeInt8 SeI2cStart(SeInt8 iIndex)
{
	SeInt8 iResult;
	SeI2cBusDescription* pI2c;

	if(iIndex < 0 || iIndex > (CONFIG_MAX_I2C_PERIPHERALS - 1))
	{
		return SE_RETURN_ERROR;
	}
	if(pI2cBuses[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	pI2c = pI2cBuses[iIndex];

	if(pI2c->blInited == SeFalse)
	{
		return SE_RETURN_ERROR;
	}
	switch(pI2c->iType)
	{
		case SE_I2C_BUS_TYPE_INTERNAL:
			iResult = SeInternalI2cStart(pI2c->pOperation);
			break;
		case SE_I2C_BUS_TYPE_SIMULATE:
			iResult = SeSimulateI2cStart(pI2c->pOperation);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeI2cStop(SeInt8 iIndex)
{
	SeInt8 iResult;
	SeI2cBusDescription* pI2c;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_I2C_PERIPHERALS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pI2cBuses[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pI2c = pI2cBuses[iIndex];

	if(pI2c->blInited == SeFalse)
	{
		return SE_RETURN_ERROR;
	}

	switch(pI2c->iType)
	{
		case SE_I2C_BUS_TYPE_INTERNAL:
			iResult = SeInternalI2cStop(pI2c->pOperation);
			break;
		case SE_I2C_BUS_TYPE_SIMULATE:
			iResult = SeSimulateI2cStop(pI2c->pOperation);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeI2cAck(SeInt8 iIndex)
{
	SeInt8 iResult;
	SeI2cBusDescription* pI2c;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_I2C_PERIPHERALS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pI2cBuses[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pI2c = pI2cBuses[iIndex];

	if(pI2c->blInited == SeFalse)
	{
		return SE_RETURN_ERROR;
	}

	switch(pI2c->iType)
	{
		case SE_I2C_BUS_TYPE_INTERNAL:
			iResult = SeInternalI2cAck(pI2c->pOperation);
			break;
		case SE_I2C_BUS_TYPE_SIMULATE:
			iResult = SeSimulateI2cAck(pI2c->pOperation);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeI2cNack(SeInt8 iIndex)
{
	SeInt8 iResult;
	SeI2cBusDescription* pI2c;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_I2C_PERIPHERALS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pI2cBuses[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pI2c = pI2cBuses[iIndex];

	if(pI2c->blInited == SeFalse)
	{
		return SE_RETURN_ERROR;
	}

	switch(pI2c->iType)
	{
		case SE_I2C_BUS_TYPE_INTERNAL:
			iResult = SeInternalI2cNack(pI2c->pOperation);
			break;
		case SE_I2C_BUS_TYPE_SIMULATE:
			iResult = SeSimulateI2cNack(pI2c->pOperation);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeI2cRead(SeInt8 iIndex, SeUInt8* bData, SeBool blAck)
{
	SeInt8 iResult;
	SeI2cBusDescription* pI2c;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_I2C_PERIPHERALS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(bData == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(pI2cBuses[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pI2c = pI2cBuses[iIndex];

	if(pI2c->blInited == SeFalse)
	{
		return SE_RETURN_ERROR;
	}

	switch(pI2c->iType)
	{
		case SE_I2C_BUS_TYPE_INTERNAL:
			iResult = SeInternalI2cRead(pI2c->pOperation, bData, blAck);
			break;
		case SE_I2C_BUS_TYPE_SIMULATE:
			iResult = SeSimulateI2cRead(pI2c->pOperation, bData, blAck);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeI2cWrite(SeInt8 iIndex, SeUInt8 bData, SeBool* blAck)
{
	SeInt8 iResult;
	SeI2cBusDescription* pI2c;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_I2C_PERIPHERALS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(blAck == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(pI2cBuses[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pI2c = pI2cBuses[iIndex];

	if(pI2c->blInited == SeFalse)
	{
		return SE_RETURN_ERROR;
	}

	switch(pI2c->iType)
	{
		case SE_I2C_BUS_TYPE_INTERNAL:
			iResult = SeInternalI2cWrite(pI2c->pOperation, bData, blAck);
			break;
		case SE_I2C_BUS_TYPE_SIMULATE:
			iResult = SeSimulateI2cWrite(pI2c->pOperation, bData, blAck);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}
