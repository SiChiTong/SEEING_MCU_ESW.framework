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
