/*
 * Gpio.c
 *
 *  Created on: 2015-12-17
 *      Author: Administrator
 */

#include "../../Manufacturer/stm32f4xx_conf.h"
#include "Gpio.h"

static GPIO_TypeDef* aGpioGroupDef[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOI};
static SeUInt16 aGpioPinDef[] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4,
		GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11,
		GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

SeInt8 SeStm32f407GpioInit(SeStm32f407GpioDescription tGpioDescription)
{

	return SE_RETURN_OK;
}

SeInt8 SeStm32f407GpioSetPin(SeStm32f407GpioPinId tGpioId)
{

	return SE_RETURN_OK;
}

SeInt8 SeStm32f407GpioClrPin(SeStm32f407GpioPinId tGpioId)
{

	return SE_RETURN_OK;
}

SeInt8 SeStm32f407GpioGetPin(SeStm32f407GpioPinId tGpioId, SeUInt8* bPinData)
{

	return SE_RETURN_OK;
}

