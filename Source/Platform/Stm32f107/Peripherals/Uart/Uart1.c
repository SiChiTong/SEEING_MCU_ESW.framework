/*
 * Uart1.c
 *
 *  Created on: 2015-12-14
 *      Author: Administrator
 */
#include "../../Manufacturer/stm32f10x_conf.h"
#include "Uart1.h"

static SeFifo* pUart1RecvFifo = SeNull;

static SeBool blUart1Inited = SeFalse;

/*
 * Uart1 irq handle
 */
void USART1_IRQHandler(void)
{
	SeUInt8 bData;

	if(blUart1Inited == SeFalse)
		return;

	/*
	 * rx interrupt
	 */
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if(SeFifoJudgeFull(pUart1RecvFifo) == SeTrue)
		{
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		}else{
			bData = USART_ReceiveData(USART1);
			SeFifoPush(pUart1RecvFifo, bData);
		}
	}
}

SeInt8 SeStm32f107Uart1Init(SeUInt32 iBaudRate)
{
	/*
	 * pins config and registers setup
	 */
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = iBaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*
	 * fifo create
	 */
	pUart1RecvFifo = SeFifoCreate(CONFIG_UART_FIFO_SIZE);
	if(pUart1RecvFifo == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	blUart1Inited = SeTrue;

	USART_Cmd(USART1, ENABLE);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_ClearFlag(USART1, USART_FLAG_TC);

	return SE_RETURN_OK;
}

SeInt8 SeStm32f107Uart1ReadByte(SeUInt8* bData)
{
	if(bData == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);

	if(SeFifoJudgeEmpty(pUart1RecvFifo) == SeTrue)
	{
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		*bData = 0x00;
		return SE_RETURN_TIMEOUT;
	}else{
		*bData = SeFifoPop(pUart1RecvFifo);
	}

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	return SE_RETURN_OK;
}

SeInt8 SeStm32f107Uart1WriteByte(SeUInt8 bData)
{
	USART_SendData(USART1, (SeUInt8) bData);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

	return SE_RETURN_OK;
}
