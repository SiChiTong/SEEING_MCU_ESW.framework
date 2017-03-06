/*
 * Gpio.c
 *
 *  Created on: 2015-12-4
 *      Author: Administrator
 */


#include "Gpio.h"

static SeGpioDescription* pGpioPins[CONFIG_MAX_USER_GPIO_PINS] = {0};

SeInt8 SeGpioInit(SeInt8 iIndex, SeGpioDescription* pGpio)
{
	SeGpioOperation* pOper = SeNull;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_USER_GPIO_PINS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pGpio == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pGpio->pOperation == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	pOper = (SeGpioOperation*)(pGpio->pOperation);

	if(pOper->fpSeGpioInit() == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	pGpioPins[iIndex] = pGpio;
	pGpioPins[iIndex]->blInited = SeTrue;

	return SE_RETURN_OK;
}

SeInt8 SeGpioSetBit(SeInt8 iIndex, SeUInt8 bBit)
{
	SeGpioOperation* pOper = SeNull;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_USER_GPIO_PINS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pGpioPins[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pGpioPins[iIndex]->blInited == SeFalse)
	{

		return SE_RETURN_ERROR;
	}

	pOper = (SeGpioOperation*)pGpioPins[iIndex]->pOperation;

	if(bBit == SE_BIT_LOW)
	{
		pOper->fpSeGpioClrPin();
	}else{
		pOper->fpSeGpioSetPin();
	}

	return SE_RETURN_OK;
}

SeInt8 SeGpioGetBit(SeInt8 iIndex, SeUInt8* bBit)
{
	SeGpioOperation* pOper = SeNull;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_USER_GPIO_PINS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(bBit == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	if(pGpioPins[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pGpioPins[iIndex]->blInited == SeFalse)
	{

		return SE_RETURN_ERROR;
	}

	pOper = (SeGpioOperation*)pGpioPins[iIndex]->pOperation;

	*bBit = pOper->fpSeGpioGetPin();

	return SE_RETURN_OK;
}
