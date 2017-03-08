/*
 * Driver.c
 *
 *  Created on: 2016年1月26日
 *      Author: seeing
 */


#include "Driver.h"
#include "Descriptions.h"
#include "../Platform/Stm32f107/Stm32f10x.h"
#include "Drivers/Encoder.h"
#include "Drivers/Motor.h"

static SeUartDescription tUserUart1Desc;
static SeUartOperation tUserUart1Oper;
static SeStdioOperation tStdioOper;

static SeGpioDescription tUserGpioRunLedDesc;
static SeGpioOperation tUserGpioRunLedOper;

/*
 * UART1 operations
 */
static SeInt8 SeUserUart1Init(SeUInt32 iBaudRate)
{
	/*
	 * pins config and registers setup
	 */
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

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

	USART_Cmd(USART1, ENABLE);

	return SE_RETURN_OK;
}

SeInt8 SeUserUart1ReadByte(SeUInt8* bData)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	*bData = USART_ReceiveData(USART1);
	return SE_RETURN_OK;
}

SeInt8 SeUserUart1WriteByte(SeUInt8 bData)
{
	USART_SendData(USART1, (SeUInt8) bData);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{}
	return SE_RETURN_OK;
}

/*
 * STDIO operations
 */
static SeInt8 SeUserStdioInit(void)
{
	return SE_RETURN_OK;
}

static SeInt8 SeUserStdioGetCh(SeUInt8* pChar)
{
	SeUInt16 nCount = 0;
	return SeUartRead(SE_UART_P1_INDEX, pChar, 1, &nCount);

}

static SeInt8 SeUserStdioPutCh(SeUInt8 bChar)
{
	SeUInt8 aBuffer[1] = {0};
	SeUInt16 nCount = 0;
	aBuffer[0] = bChar;

	return SeUartWrite(SE_UART_P1_INDEX, aBuffer, 1, &nCount);
}

/*
 * system driver initial function
 */
SeInt8 SeUserDriverInit(void)
{

	NVIC_SetVectorTable(((SeUInt32)0x08000000), 0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	tUserUart1Oper.fpSeUartPreInit = SeStm32f107Uart1Init;
	tUserUart1Oper.fpSeUartReadByte = SeStm32f107Uart1ReadByte;
	tUserUart1Oper.fpSeUartWriteByte = SeStm32f107Uart1WriteByte;

	tUserUart1Desc.iBaudRate = 115200;
	tUserUart1Desc.pOperation = &tUserUart1Oper;
	if(SeUartInit(SE_UART_P1_INDEX, &tUserUart1Desc) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	tStdioOper.fpSeStdioInit = SeUserStdioInit;
	tStdioOper.fpSeFgetc = SeUserStdioGetCh;
	tStdioOper.fpSeFputc = SeUserStdioPutCh;
	if(SeStdConfig(&tStdioOper) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("Console configure ok!");

	return SE_RETURN_OK;
}
