/*
 * Spi2Slave.c
 *
 *  Created on: 2017年2月22日
 *      Author: seeing
 */

#include "Spi2Slave.h"
#include "../../Manufacturer/stm32f10x_conf.h"

static SeSpiSlaveBufferDescription tGlobalBufferDesc;
static SeSpiSlaveCallback tGlobalCallbacks;

static void SeStm32f107Spi2SlaveTxConfig(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&SPI2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)tGlobalBufferDesc.pTransmitBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = tGlobalBufferDesc.nTransmitBufferLength;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);

	SPI_I2S_DMACmd(SPI2,SPI_CR2_TXDMAEN, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(DMA1_IT_TC5);

	DMA_Cmd(DMA1_Channel5, DISABLE);
}

static void SeStm32f107Spi2SlaveRxConfig(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&SPI2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)tGlobalBufferDesc.pReceiveBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = tGlobalBufferDesc.nReceiveBufferLength;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	SPI_I2S_DMACmd(SPI2,SPI_CR2_RXDMAEN, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

	DMA_ClearITPendingBit(DMA1_IT_TC4);

	DMA_Cmd(DMA1_Channel4, ENABLE);
}

static void SeStm32f107Spi2SlaveResetDMA(void)
{
	SeStm32f107Spi2SlaveTxConfig();
	SeStm32f107Spi2SlaveRxConfig();

	SPI_Cmd(SPI2, ENABLE);
}

static void SeStm32f107Spi2SlaveHwInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_CalculateCRC(SPI2, DISABLE);
	SPI_Init(SPI2, &SPI_InitStructure);

}

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4) == SET)
	{
		SeInt16 nResponseLength;
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		tGlobalCallbacks.fpSeSpiSlaveReceiveRequest(tGlobalBufferDesc.pReceiveBuffer, tGlobalBufferDesc.nReceiveBufferLength);
		tGlobalCallbacks.fpSeSpiSlaveMakeResponse(tGlobalBufferDesc.pTransmitBuffer, &nResponseLength);
		DMA_Cmd(DMA1_Channel5, ENABLE);
		DMA_Cmd(DMA1_Channel4, DISABLE);
	}
}

void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5) == SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC5);
		tGlobalCallbacks.fpSeSpiSlaveLaterProcess();
		SeStm32f107Spi2SlaveResetDMA();
	}
}

SeInt8 SeStm32f107Spi2SlaveInit(SeSpiSlaveBufferDescription tBufferDesc, SeSpiSlaveCallback tCallbacks)
{
	if(tBufferDesc.pReceiveBuffer == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(tBufferDesc.pTransmitBuffer == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(tCallbacks.fpSeSpiSlaveReceiveRequest == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(tCallbacks.fpSeSpiSlaveMakeResponse == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(tCallbacks.fpSeSpiSlaveLaterProcess == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	tGlobalBufferDesc = tBufferDesc;

	tGlobalCallbacks = tCallbacks;

	SeStm32f107Spi2SlaveHwInit();

	SeStm32f107Spi2SlaveResetDMA();

	SeDebugPrint("Spi HAL init ok!");

	return SE_RETURN_OK;
}
