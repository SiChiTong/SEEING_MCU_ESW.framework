/*
 * Gpio.h
 *
 *  Created on: 2015-12-17
 *      Author: Administrator
 */

#ifndef _STM32F407_GPIO_H_
#define _STM32F407_GPIO_H_

#include "../../../../Global/Include.h"
typedef enum
{
	SeStm32f407GpioGroupA = 0,
	SeStm32f407GpioGroupB,
	SeStm32f407GpioGroupC,
	SeStm32f407GpioGroupD,
	SeStm32f407GpioGroupE,
	SeStm32f407GpioGroupF,
	SeStm32f407GpioGroupG,
	SeStm32f407GpioGroupI,
}SeStm32f407GpioGroup;

typedef enum
{
	SeStm32f407GpioPin0 = 0,
	SeStm32f407GpioPin1,
	SeStm32f407GpioPin2,
	SeStm32f407GpioPin3,
	SeStm32f407GpioPin4,
	SeStm32f407GpioPin5,
	SeStm32f407GpioPin6,
	SeStm32f407GpioPin7,
	SeStm32f407GpioPin8,
	SeStm32f407GpioPin9,
	SeStm32f407GpioPin10,
	SeStm32f407GpioPin11,
	SeStm32f407GpioPin12,
	SeStm32f407GpioPin13,
	SeStm32f407GpioPin14,
	SeStm32f407GpioPin15,
}SeStm32f407GpioPin;

typedef enum
{
	SeStm32f407GpioInput = 0x00,
	SeStm32f407GpioOutput
}SeStm32f407GpioMode;

typedef enum
{
	SeStm32f407GpioOutputPushPull = 0x00,
	SeStm32f407GpioOutputOpenDrain,
}SeStm32f407GpioOutputType;

typedef enum
{
	SeStm32f407GpioNoPull = 0x00,
	SeStm32f407GpioPullUp,
	SeStm32f407GpioPullDown,
}SeStm32f407GpioPullType;

typedef enum
{
	SeStm32f407Gpio2MHz = 0,
	SeStm32f407Gpio25MHz,
	SeStm32f407Gpio50MHz,
	SeStm32f407Gpio100MHz,
}SeStm32f407GpioSpeed;

typedef struct
{
	SeStm32f407GpioGroup tGpioGroup;
	SeStm32f407GpioPin tGpioPin;
}SeStm32f407GpioPinId;

typedef struct
{
	SeStm32f407GpioPinId tGpioId;
	SeStm32f407GpioMode tGpioMode;
	SeStm32f407GpioOutputType tGpioOutputType;
	SeStm32f407GpioPullType tGpioPullType;
	SeStm32f407GpioSpeed tGpioSpeed;
}SeStm32f407GpioDescription;

SeInt8 SeStm32f407GpioInit(SeStm32f407GpioDescription tGpioDescription);
SeInt8 SeStm32f407GpioSetPin(SeStm32f407GpioPinId tGpioId);
SeInt8 SeStm32f407GpioClrPin(SeStm32f407GpioPinId tGpioId);
SeInt8 SeStm32f407GpioGetPin(SeStm32f407GpioPinId tGpioId, SeUInt8* bPinData);

#endif /* _STM32F407_GPIO_H_ */
