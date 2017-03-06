/*
 * Driver.c
 *
 *  Created on: 2016年1月26日
 *      Author: seeing
 */


#include "../User_ForArmBoard407/Driver.h"

#include "../Platform/Stm32f407/Stm32f407.h"
#include "../User_ForArmBoard407/Descriptions.h"

static SeUartDescription tUserUart1Desc;
static SeUartOperation tUserUart1Oper;
static SeStdioOperation tStdioOper;

static SeGpioDescription tUserGpioRunLedDesc;
static SeGpioOperation tUserGpioRunLedOper;

static SeW25xDescription tW25X16Desc;
static SeSpiBusDescription tW25X16FlashDesc;
static SeSimulateSpiOperation tW25X16FlashOper;
static SeGpioDescription tW25X16WriteProtectDesc;
static SeGpioOperation tW25X16WriteProtectOper;
static SeGpioDescription tW25X16CsDesc;
static SeGpioOperation tW25X16CsOper;

static SeAt24cxxDescription tAt24c02Desc;
static SeI2cBusDescription tAt24c02E2promDesc;
static SeSimulateI2cOperation tAt24c02E2promOper;

static SeCat9555Description tCat9555Desc;
static SeI2cBusDescription tCat9555BusDesc;
static SeSimulateI2cOperation tCat9555BusOper;

static SeFpgaDescription tFpgaDesc;
static SeSpiBusDescription tFpga1WireSpiDesc;
static SeSimulateSpiOperation tFpga1WireSpiOper;
static SeSpiBusDescription tFpgaQuadBusDesc;
static SeQuadSpiOperation tFpgaQuadBusOper;
static SeGpioDescription tFpgaCsDesc;
static SeGpioOperation tFpgaCsOper;

static SeXilinxFpgaConfigOperation tFpgaConfigOper;

static SeUpdateDescription tUpdateDesc;

static SeEthernetPhyDescription tPhyDesc;
static SeEthernetDescription tEthDesc;

static SeXmodemDescription tXmodemDesc;

static SeTftpClientGetFileOperation tTftpUpdateDesc;

/*
 * UART1 operations
 */
static SeInt8 SeUserUart1Init(SeUInt32 iBaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = iBaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ClearFlag(USART1, USART_FLAG_TC);

	USART_Cmd(USART1, ENABLE);
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
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
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
 * RUN LED operations
 */
static SeInt8 SeUserRunLedInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz ;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static void SeUserRunLedSetPin(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
}

static void SeUserRunLedClrPin(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}

/*
 * W25X16FLASH operations
 */
static SeInt8 SeUserW25X16FlashInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
	/* SCK */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* CS */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* MISO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	return SE_RETURN_OK;
}

static void SeUserW25X16FlashSetCs(void)
{
	//GPIO_SetBits(GPIOA, GPIO_Pin_15);
}

static void SeUserW25X16FlashClrCs(void)
{
	//GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}

static void SeUserW25X16FlashSetSclk(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	SeDelayCycles(1);
}

static void SeUserW25X16FlashClrSclk(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	SeDelayCycles(1);
}

static void SeUserW25X16FlashSetSdo(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_3);
}

static void SeUserW25X16FlashClrSdo(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_3);
}

static SeUInt8 SeUserW25X16FlashGetSdi(void)
{
	return  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
}

static SeInt8 SeUserW25X16FlashWpInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	return SE_RETURN_OK;
}

static void SeUserW25X16FlashWpSetPin(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

static void SeUserW25X16FlashWpClrPin(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

static SeInt8 SeUserW25X16CsPinInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	return SE_RETURN_OK;
}

static void SeUserW25X16FlashCsSetPin(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
}

static void SeUserW25X16FlashCsClrPin(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}

/*
 *AT24C02 E2PROM operations
 */
static SeInt8 SeUserAt24c02E2promInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* SCK */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* SDA */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,  GPIO_Pin_0);
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	return SE_RETURN_OK;
}

static SeInt8 SeUserAt24c02GetSda(void)
{
	SeUInt8 iState;

	GPIOB->MODER &= ~(3<<(3*2));
	GPIOB->MODER |= 0<<(3*2);
	iState = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
	GPIOB->MODER &= ~(3<<(3*2));
	GPIOB->MODER |= 1<<(3*2);
	GPIOB->OTYPER &= ~(1<<3);
	GPIOB->OTYPER |= 1<<3;
//	printf("istate = %d\r\n", iState);
	return iState;
}

static void SeUserAt24c02SetSda(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	SeDelayCycles(500);
}

static void SeUserAt24c02ClrSda(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	SeDelayCycles(500);
}

static void SeUserAt24c02SetSclk(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	SeDelayCycles(500);
}

static void SeUserAt24c02ClrSclk(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	SeDelayCycles(500);
}

/*
 *Cat9555 operations
 */
static SeInt8 SeUserCat9555GpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* SCK */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* SDA */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,  GPIO_Pin_0);
	GPIO_SetBits(GPIOD, GPIO_Pin_1);
	return SE_RETURN_OK;
}

static SeInt8 SeUserCat9555GetSda(void)
{
	SeUInt8 iState;

	iState = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0);
	return iState;
}

static void SeUserCat9555SetSda(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_0);
	SeDelayCycles(500);
}

static void SeUserCat9555ClrSda(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_0);
	SeDelayCycles(500);
}

static void SeUserCat9555SetSclk(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_1);
	SeDelayCycles(500);
}

static void SeUserCat9555ClrSclk(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_1);
	SeDelayCycles(500);
}

/*
 *FPGA QUAD BUS operations
 */
static SeInt8 SeUserFpgaQuadBusGpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);

	/* SCK */
	/* PB10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	 /* MOSI */
	/* Dout0 PE8 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Dout1 PE9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Dout2 PE10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Dout3 PE11 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	 /* MISO */
	/* Din0 PE0 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Din1 PE1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Din2 PE2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Din3 PE3 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	return SE_RETURN_OK;
}

static void SeUserFpgaQuadBusSetCs(void)
{
}

static void SeUserFpgaQuadBusClrCs(void)
{
}

static void SeUserFpgaQuadBusSetSclk(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	SeDelayCycles(4);
}

static void SeUserFpgaQuadBusClrSclk(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	SeDelayCycles(4);
}

static void SeUserFpgaQuadBusSetSdo(SeQuadSpiData tData)
{
	if(tData.Bit.D0 == SE_BIT_HIGH)
		GPIO_SetBits(GPIOE, GPIO_Pin_8);
	else
		GPIO_ResetBits(GPIOE, GPIO_Pin_8);
	if(tData.Bit.D1 == SE_BIT_HIGH)
		GPIO_SetBits(GPIOE, GPIO_Pin_9);
	else
		GPIO_ResetBits(GPIOE, GPIO_Pin_9);
	if(tData.Bit.D2 == SE_BIT_HIGH)
		GPIO_SetBits(GPIOE, GPIO_Pin_10);
	else
		GPIO_ResetBits(GPIOE, GPIO_Pin_10);
	if(tData.Bit.D3 == SE_BIT_HIGH)
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
	else
		GPIO_ResetBits(GPIOE, GPIO_Pin_11);
}

static SeQuadSpiData SeUserFpgaQuadBusGedSdi(void)
{
	SeQuadSpiData tSpiData;
	SeUInt8 iTemp = 0;
	iTemp |= GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);
	iTemp = iTemp << 1;
	iTemp |= GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2);
	iTemp = iTemp << 1;
	iTemp |= GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1);
	iTemp = iTemp << 1;
	iTemp |= GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
	tSpiData.Byte = iTemp;
	return tSpiData;
}

static SeInt8 SeUserFpgaCsPinInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	return SE_RETURN_OK;
}

static void SeUserFpgaCsSetPin(void)
{
	SeDelayCycles(40);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

static void SeUserFpgaCsClrPin(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	SeDelayCycles(40);
}

static SeInt8 SeUserFpga1WireGpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);

	/* SCK */
	/* PB10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	 /* MOSI */
	/* Dout0 PE8 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	 /* MISO */
	/* Din0 PE0 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	return SE_RETURN_OK;
}

static void SeUserFpga1WireSetCs(void)
{
}

static void SeUserFpga1WireClrCs(void)
{
}

static void SeUserFpga1WireSetSclk(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	SeDelayCycles(40);
}

static void SeUserFpga1WireClrSclk(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	SeDelayCycles(40);
}

static void SeUserFpga1WireSetSdo(void)
{
	GPIO_SetBits(GPIOE, GPIO_Pin_8);
}

static void SeUserFpga1WireClrSdo(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_8);
}

static SeUInt8 SeUserFpga1WireGetSdi(void)
{
	return  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
}

/*
 * FPGA config operations
 */
static SeInt8 SeUserFpgaDownloadGpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* SOFT_RST */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* PROGRAM_B */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* CLOCK */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* DOUT*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* INIT_B */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* DONE */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_SetBits(GPIOD, GPIO_Pin_9);
	GPIO_SetBits(GPIOD, GPIO_Pin_10);
	GPIO_SetBits(GPIOD, GPIO_Pin_11);
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
	return SE_RETURN_OK;
}

static SeInt8 SeUserFpgaGetRomData(SeUInt32 iSeekSourcePosition, SeUInt8* pDestination, SeUInt32 nBytesToRead, SeUInt32* nBytesReaded)
{
	SeUInt32 hAddress = 0;
	SeUInt8 aBuffer[SE_BUFFER_SIZE_256] = {0};

	if(iSeekSourcePosition == 8)
	{
		hAddress = 0;
	}else{
		hAddress = iSeekSourcePosition;
	}

	if(SeW25xSpiFlashFastReadData(&tW25X16Desc, hAddress, aBuffer, SE_BUFFER_SIZE_256) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	if(iSeekSourcePosition == 8)
	{
		memcpy(pDestination, &aBuffer[8], (nBytesToRead - 8));
		*nBytesReaded = (nBytesToRead - 8);
	}else{
		memcpy(pDestination, aBuffer, nBytesToRead);
		*nBytesReaded = nBytesToRead;
	}

	return SE_RETURN_OK;
}

static void SeUserFpgaSetProgb(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_11);
	SeDelayCycles(2);
}

static void SeUserFpgaClrProgb(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_11);
	SeDelayCycles(1);
}

static void SeUserFpgaSetCs(void)
{
}

static void SeUserFpgaClrCs(void)
{
}

static void SeUserFpgaSetRst(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_9);
}

static void SeUserFpgaClrRst(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_9);
	SeDelayCycles(2);
}

static void SeUserFpgaSetClk(void)
{
	//SeDelayCycles(1);
	GPIO_SetBits(GPIOD, GPIO_Pin_10);
	//SeDelayCycles(1);
}

static void SeUserFpgaClrClk(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_10);
}

static void SeUserFpgaSetDout(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
}

static void SeUserFpgaClrDout(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_12);
}

static SeUInt8 SeUserFpgaGetInitb(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14) == SE_BIT_HIGH)
	{
		SeDelayCycles(10);
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14) == SE_BIT_HIGH)
		{
			return SE_BIT_HIGH;
		}
	}
	SeDelayCycles(2);
	return  SE_BIT_LOW;
}

static SeUInt8 SeUserFpgaGetDone(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15) == SE_BIT_HIGH)
	{
		SeDelayCycles(10);
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15) == SE_BIT_HIGH)
		{
			return SE_BIT_HIGH;
		}
	}
	SeDelayCycles(2);
	return  SE_BIT_LOW;
}

static SeUInt8 SeUserFpgaGetDin(void)
{
}

static SeInt8 SeUpdateMcuByXmodem(void)
{
	SeInt8 iRet;
	SeUInt32 nBytesReceived;
	SeUInt32 nMcuStorageSize = 128*1024*2;
	SeUInt32 nMcuStorageBaseAddr = 0x080C0000;

	SeStm32f407FlashUnlock();
	SeStm32f407FlashClearFlag();
	SeStm32f407FlashEraseSector(FLASH_Sector_10);
	SeStm32f407FlashEraseSector(FLASH_Sector_11);

	iRet = SeXmodemReceive(nMcuStorageSize, &nBytesReceived,
			nMcuStorageBaseAddr, SeStm32f407FlashWritedata);

	if(iRet == SE_RETURN_ERROR)
	{
		SeStm32f407FlashEraseSector(FLASH_Sector_10);
		SeStm32f407FlashEraseSector(FLASH_Sector_11);
		return SE_RETURN_ERROR;
	}

	SeStm32f407FlashLock();
	return SE_RETURN_OK;
}

static SeInt8 SeW25xFlashWritedata(SeUInt32 nAddress, SeUInt8* pData, SeUInt16 nLength)
{
	return SeW25xSpiFlashWriteData(&tW25X16Desc, nAddress, pData, nLength);
}

static SeInt8 SeUpdateFpgaByXmodem(void)
{
	SeUInt32 nBytesReceived;
	SeUInt32 nFpagStorageSize = W25X_CHIP_SIZE;
	SeUInt32 nFpgaStorageBaseAddr = 0x00;

	SeW25xSpiFlashChipErase(&tW25X16Desc, 0);

	return SeXmodemReceive(nFpagStorageSize, &nBytesReceived,
			nFpgaStorageBaseAddr, SeW25xFlashWritedata);
}

/*
 * TFTP update functions
 */
static SeUpdateApplicationType tUpdateAppType;
static SeInt8 SeUserTftpClientOpen(SeString sDescription, SeUpdateApplicationType tUpdateApplicationType)
{
	tUpdateAppType = tUpdateApplicationType;
	if(tUpdateAppType == SeUpdateApplicationTypeFpga)
	{
		SeW25xSpiFlashChipErase(&tW25X16Desc, 0);
	}else if(tUpdateAppType == SeUpdateApplicationTypeMcu)
	{
		SeStm32f407FlashUnlock();
		SeStm32f407FlashClearFlag();
		SeStm32f407FlashEraseSector(FLASH_Sector_10);
		SeStm32f407FlashEraseSector(FLASH_Sector_11);
	}

	return SE_RETURN_OK;
}

static SeInt8 SeUserTftpClientWrite(SeUInt32 iPosition, SeUInt8* pData, SeUInt16 nLength)
{
	if(tUpdateAppType == SeUpdateApplicationTypeFpga)
	{
		SeW25xSpiFlashWriteData(&tW25X16Desc, iPosition, pData, nLength);
	}
}

static SeInt8 SeUserTftpClientClose(void)
{
	if(tUpdateAppType == SeUpdateApplicationTypeMcu)
	{
		SeStm32f407FlashLock();
	}
}

/*
 * system driver initial function
 */
SeInt8 SeUserDriverInit(void)
{
	NVIC_SetVectorTable(((SeUInt32)0x08000000), 0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	tUserGpioRunLedOper.fpSeGpioInit = SeUserRunLedInit;
	tUserGpioRunLedOper.fpSeGpioSetPin = SeUserRunLedSetPin;
	tUserGpioRunLedOper.fpSeGpioClrPin = SeUserRunLedClrPin;
	tUserGpioRunLedDesc.pOperation = &tUserGpioRunLedOper;
	if(SeGpioInit(SE_GPIO_RUN_LED_INDEX, &tUserGpioRunLedDesc) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	tUserUart1Oper.fpSeUartPreInit = SeUserUart1Init;
	tUserUart1Oper.fpSeUartReadByte = SeUserUart1ReadByte;
	tUserUart1Oper.fpSeUartWriteByte = SeUserUart1WriteByte;

	/*
	tUserUart1Oper.fpSeUartPreInit = SeStm32f407Uart1Init;
	tUserUart1Oper.fpSeUartReadByte = SeStm32f407Uart1ReadByte;
	tUserUart1Oper.fpSeUartWriteByte = SeStm32f407Uart1WriteByte;
*/
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

	tW25X16FlashOper.fpSeSimulateSpiPreInit = SeUserW25X16FlashInit;
	tW25X16FlashOper.fpSeSimulateSpiSetCs = SeUserW25X16FlashSetCs;
	tW25X16FlashOper.fpSeSimulateSpiClrCs = SeUserW25X16FlashClrCs;
	tW25X16FlashOper.fpSeSimulateSpiSetScl = SeUserW25X16FlashSetSclk;
	tW25X16FlashOper.fpSeSimulateSpiClrScl = SeUserW25X16FlashClrSclk;
	tW25X16FlashOper.fpSeSimulateSpiSetSdo = SeUserW25X16FlashSetSdo;
	tW25X16FlashOper.fpSeSimulateSpiClrSdo = SeUserW25X16FlashClrSdo;
	tW25X16FlashOper.fpSeSimulateSpiGetSdi = SeUserW25X16FlashGetSdi;
	tW25X16FlashDesc.iType = SE_SPI_BUS_TYPE_SIMULATE;
	tW25X16FlashDesc.pOperation = &tW25X16FlashOper;
	if(SeSpiInit(SE_W25X_FLASH_INDEX, &tW25X16FlashDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("W25X SPI initial fail!");
		return SE_RETURN_ERROR;
	}
	tW25X16CsOper.fpSeGpioInit = SeUserW25X16CsPinInit;
	tW25X16CsOper.fpSeGpioSetPin = SeUserW25X16FlashCsSetPin;
	tW25X16CsOper.fpSeGpioClrPin = SeUserW25X16FlashCsClrPin;
	tW25X16CsDesc.pOperation = &tW25X16CsOper;
	if(SeGpioInit(SE_GPIO_W25X_CS_INDEX, &tW25X16CsDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("W25X CS initial fail!");
		return SE_RETURN_ERROR;
	}
	tW25X16WriteProtectOper.fpSeGpioInit = SeUserW25X16FlashWpInit;
	tW25X16WriteProtectOper.fpSeGpioSetPin = SeUserW25X16FlashWpSetPin;
	tW25X16WriteProtectOper.fpSeGpioClrPin = SeUserW25X16FlashWpClrPin;
	tW25X16WriteProtectDesc.pOperation = &tW25X16WriteProtectOper;
	if(SeGpioInit(SE_GPIO_W25X_WP_INDEX, &tW25X16WriteProtectDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("W25X WP initial fail!");
		return SE_RETURN_ERROR;
	}
	tW25X16Desc.iW25xSpiIndex = SE_W25X_FLASH_INDEX;
	tW25X16Desc.iW25xWriteProtectPinIndex = SE_GPIO_W25X_WP_INDEX;
	tW25X16Desc.iW25xCsPinIndex = SE_GPIO_W25X_CS_INDEX;
	if(SeW25xSpiFlashInit(&tW25X16Desc) != SE_RETURN_OK)
	{
		SeErrorPrint("W25X initial fail!");
		return SE_RETURN_ERROR;
	}
	SeNormalPrint("W25X flash configure ok!");

	/*
	tAt24c02E2promOper.fpSeSimulateI2cPreInit = SeUserAt24c02E2promInit;
	tAt24c02E2promOper.fpSeSimulateI2cGetSda = SeUserAt24c02GetSda;
	tAt24c02E2promOper.fpSeSimulateI2cSetSda = SeUserAt24c02SetSda;
	tAt24c02E2promOper.fpSeSimulateI2cClrSda = SeUserAt24c02ClrSda;
	tAt24c02E2promOper.fpSeSimulateI2cSetScl = SeUserAt24c02SetSclk;
	tAt24c02E2promOper.fpSeSimulateI2cClrScl = SeUserAt24c02ClrSclk;
	tAt24c02E2promDesc.iType = SE_I2C_BUS_TYPE_SIMULATE;
	tAt24c02E2promDesc.pOperation = &tAt24c02E2promOper;
	if(SeI2cInit(SE_AT24C02_E2PROM_INDEX, &tAt24c02E2promDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("AT24 SPI initial fail!");
		return SE_RETURN_ERROR;
	}
	tAt24c02Desc.iAt24cxxI2cIndex = SE_AT24C02_E2PROM_INDEX;
	tAt24c02Desc.hAt24cxxDeviceAddress = 0xa4;
	tAt24c02Desc.tAt24cxxType = At24c02;
	tAt24c02Desc.iAt24cxxChipSize = 256;
	//tAt24c02Desc.tAt24cxxType = At24c08;
	//tAt24c02Desc.iAt24cxxChipSize = 1024;
	if(SeAt24cxxE2promInit(&tAt24c02Desc) != SE_RETURN_OK)
	{
		SeErrorPrint("AT24 initial fail!");
		return SE_RETURN_ERROR;
	}
	SeNormalPrint("AT24X EEPROM configure ok!");
	*/

	/*
	tCat9555BusOper.fpSeSimulateI2cPreInit = SeUserCat9555GpioInit;
	tCat9555BusOper.fpSeSimulateI2cGetSda = SeUserCat9555GetSda;
	tCat9555BusOper.fpSeSimulateI2cSetSda = SeUserCat9555SetSda;
	tCat9555BusOper.fpSeSimulateI2cClrSda = SeUserCat9555ClrSda;
	tCat9555BusOper.fpSeSimulateI2cSetScl = SeUserCat9555SetSclk;
	tCat9555BusOper.fpSeSimulateI2cClrScl = SeUserCat9555ClrSclk;
	tCat9555BusDesc.iType = SE_I2C_BUS_TYPE_SIMULATE;
	tCat9555BusDesc.pOperation = &tCat9555BusOper;
	if(SeI2cInit(SE_CAT9555_INDEX, &tCat9555BusDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("CAT9555 I2C initial fail!");
		return SE_RETURN_ERROR;
	}
	tCat9555Desc.iCat9555I2cIndex = SE_CAT9555_INDEX;
	tCat9555Desc.hCat9555DeviceAddress =(0x20|0x4)<<1;
	if(SeCat9555Init(&tCat9555Desc) != SE_RETURN_OK)
	{
		SeErrorPrint("CAT9555 initial fail!");
		return SE_RETURN_ERROR;
	}
	SeNormalPrint("CAT9555  configure ok!");
	if(SeCat9555InputOutputConfig(&tCat9555Desc, 0x0000) != SE_RETURN_OK)
	{
		SeErrorPrint("CAT9555 input output configure fail!");
		return SE_RETURN_ERROR;
	}
	if(SeCat9555PinsSetLow(&tCat9555Desc, 0x200e) != SE_RETURN_OK)
	{
		SeErrorPrint("CAT9555 pins set low fail!");
		return SE_RETURN_ERROR;
	}
	if(SeCat9555ReadOutPortRegistorValue(&tCat9555Desc, &bTemp) != SE_RETURN_OK)
	{
		SeErrorPrint("CAT9555 read reg fail!");
		return SE_RETURN_ERROR;
	}
	 */

	tFpga1WireSpiOper.fpSeSimulateSpiPreInit = SeUserFpga1WireGpioInit;
	tFpga1WireSpiOper.fpSeSimulateSpiSetCs = SeUserFpga1WireSetCs;
	tFpga1WireSpiOper.fpSeSimulateSpiClrCs = SeUserFpga1WireClrCs;
	tFpga1WireSpiOper.fpSeSimulateSpiSetScl = SeUserFpga1WireSetSclk;
	tFpga1WireSpiOper.fpSeSimulateSpiClrScl = SeUserFpga1WireClrSclk;
	tFpga1WireSpiOper.fpSeSimulateSpiSetSdo = SeUserFpga1WireSetSdo;
	tFpga1WireSpiOper.fpSeSimulateSpiClrSdo = SeUserFpga1WireClrSdo;
	tFpga1WireSpiOper.fpSeSimulateSpiGetSdi = SeUserFpga1WireGetSdi;
	tFpga1WireSpiDesc.iType = SE_SPI_BUS_TYPE_SIMULATE;
	tFpga1WireSpiDesc.pOperation = &tFpga1WireSpiOper;
	if(SeSpiInit(SE_FPGA_1_WIRE_INDEX, &tFpga1WireSpiDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("FPGA 1-wire SPI initial fail!");
		return SE_RETURN_ERROR;
	}

	tFpgaQuadBusOper.fpSeQuadSpiPreInit = SeUserFpgaQuadBusGpioInit;
	tFpgaQuadBusOper.fpSeQuadSpiSetCs = SeUserFpgaQuadBusSetCs;
	tFpgaQuadBusOper.fpSeQuadSpiClrCs = SeUserFpgaQuadBusClrCs;
	tFpgaQuadBusOper.fpSeQuadSpiSetScl = SeUserFpgaQuadBusSetSclk;
	tFpgaQuadBusOper.fpSeQuadSpiClrScl = SeUserFpgaQuadBusClrSclk;
	tFpgaQuadBusOper.fpSeQuadSpiSetSdo = SeUserFpgaQuadBusSetSdo;
	tFpgaQuadBusOper.fpSeQuadSpiGetSdi = SeUserFpgaQuadBusGedSdi;
	tFpgaQuadBusDesc.iType = SE_SPI_BUS_TYPE_QUAD;
	tFpgaQuadBusDesc.pOperation = &tFpgaQuadBusOper;
	if(SeSpiInit(SE_FPGA_4_WIRE_INDEX, &tFpgaQuadBusDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("FPGA 4-wire SPI initial fail!");
		return SE_RETURN_ERROR;
	}
	tFpgaCsOper.fpSeGpioInit = SeUserFpgaCsPinInit;
	tFpgaCsOper.fpSeGpioSetPin = SeUserFpgaCsSetPin;
	tFpgaCsOper.fpSeGpioClrPin = SeUserFpgaCsClrPin;
	tFpgaCsDesc.pOperation = &tFpgaCsOper;
	if(SeGpioInit(SE_GPIO_FPGA_CS_INDEX, &tFpgaCsDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("FPGA CS initial fail!");
		return SE_RETURN_ERROR;
	}
	tFpgaDesc.iFpgaInfoSpiIndex = SE_FPGA_1_WIRE_INDEX;
	tFpgaDesc.iFpgaCsPinIndex = SE_GPIO_FPGA_CS_INDEX;
	tFpgaDesc.iFpgaDataSpiIndex = SE_FPGA_4_WIRE_INDEX;
	if(SeFpgaInit(tFpgaDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("FPGA initial fail!");
		return SE_RETURN_ERROR;
	}
	SeNormalPrint("FPGA bus configure ok!");

	tFpgaConfigOper.fpSeXiFpgaCfgInit = SeUserFpgaDownloadGpioInit;
	tFpgaConfigOper.fpSeXiFpgaGetRomData = SeUserFpgaGetRomData;
	tFpgaConfigOper.fpSeXiFpgaSetProgb = SeUserFpgaSetProgb;
	tFpgaConfigOper.fpSeXiFpgaClrProgb = SeUserFpgaClrProgb;
	tFpgaConfigOper.fpSeXiFpgaGetInitb = SeUserFpgaGetInitb;
	tFpgaConfigOper.fpSeXiFpgaSetCs = SeUserFpgaSetCs;
	tFpgaConfigOper.fpSeXiFpgaClrCs = SeUserFpgaClrCs;
	tFpgaConfigOper.fpSeXiFpgaSetRst = SeUserFpgaSetRst;
	tFpgaConfigOper.fpSeXiFpgaClrRst = SeUserFpgaClrRst;
	tFpgaConfigOper.fpSeXiFpgaSetClk = SeUserFpgaSetClk;
	tFpgaConfigOper.fpSeXiFpgaClrClk = SeUserFpgaClrClk;
	tFpgaConfigOper.fpSeXiFpgaGetDone = SeUserFpgaGetDone;
	tFpgaConfigOper.fpSeXiFpgaGetDin = SeUserFpgaGetDin;
	tFpgaConfigOper.fpSeXiFpgaSetDout = SeUserFpgaSetDout;
	tFpgaConfigOper.fpSeXiFpgaClrDout = SeUserFpgaClrDout;
	if(SeConfigXilinxFpga(tFpgaConfigOper) != SE_RETURN_OK)
	{
		SeErrorPrint("FPGA initial fail!");
		//return SE_RETURN_ERROR;
	}
	SeNormalPrint("FPGA configure ok!");

	tUpdateDesc.fpSeUpdateMcu = SeUpdateMcuByXmodem;
	tUpdateDesc.fpSeUpdateFpga = SeUpdateFpgaByXmodem;
	SeUpdateConfig(tUpdateDesc);
	SeNormalPrint("Update configure ok!");

	tXmodemDesc.iXmodemUartIndex = SE_UART_P1_INDEX;
	SeXmodemConfig(tXmodemDesc);

	/*
	 * tftp update configure
	 */
	tTftpUpdateDesc.fpSeTftpClientOpen = SeUserTftpClientOpen;
	tTftpUpdateDesc.fpSeTftpClientWrite = SeUserTftpClientWrite;
	tTftpUpdateDesc.fpSeTftpClientClose = SeUserTftpClientClose;
	SeTftpUpdateOperationSet(tTftpUpdateDesc);

	/*
	 * ethernet configure
	 */
	tPhyDesc.fpSeEthernetPhyPreInit = SeDp83848PhyInit;
	tPhyDesc.fpSeEthernetPhyReset = SeDp83848PhyReset;
	tPhyDesc.fpSeEthernetGetLinkSpeed = SeDp83848PhyGetLinkSpeed;
	tPhyDesc.fpSeEthernetIsLinked = SeDp83848PhyGetIsLinked;

	tEthDesc.pEthernetPhyDescription = &tPhyDesc;
	tEthDesc.fpSeEthernetInit = SeStm32f407MacInit;
	if(SeEthernetInit(&tEthDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("Ethernet initial fail!");
		return SE_RETURN_ERROR;
	}
	SeNormalPrint("Ethernet configure ok!");


	return SE_RETURN_OK;
}
