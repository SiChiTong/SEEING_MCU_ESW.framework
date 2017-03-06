/*
 * Spi2Slave.h
 *
 *  Created on: 2017年2月22日
 *      Author: seeing
 */

#ifndef _STM32F107_SPI_SPI2SLAVE_H_
#define _STM32F107_SPI_SPI2SLAVE_H_

#include "../../../../Global/Include.h"

SeInt8 SeStm32f107Spi2SlaveInit(SeSpiSlaveBufferDescription tBufferDesc, SeSpiSlaveCallback tCallbacks);

#endif /* SOURCE_PLATFORM_STM32F107_PERIPHERALS_SPI_SPI2SLAVE_H_ */
