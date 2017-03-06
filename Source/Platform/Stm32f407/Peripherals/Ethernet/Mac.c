/*
 * Mac.c
 *
 *  Created on: 2016年2月24日
 *      Author: seeing
 */

#include "Mac.h"
#include "Dp83848.h"
#include "../../Manufacturer/stm32f4xx_conf.h"
#include "stm32f4x7_eth.h"

/* MII and RMII mode selection, for STM324xG-EVAL Board(MB786) RevB ***********/
#define RMII_MODE  // User have to provide the 50 MHz clock by soldering a 50 MHz
                     // oscillator (ref SM7745HEV-50.0M or equivalent) on the U3
                     // footprint located under CN3 and also removing jumper on JP5.
                     // This oscillator is not provided with the board.
                     // For more details, please refer to STM3240G-EVAL evaluation
                     // board User manual (UM1461).


//#define MII_MODE
/* Uncomment the define below to clock the PHY from external 25MHz crystal (only for MII mode) */
#ifdef 	MII_MODE
#define PHY_CLOCK_MCO
#endif

SeInt8 SeStm32f407MacInit(void)
{
	/*
	 * GPIO configure
	 */
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
			RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOD, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure MCO (PA8) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* MII/RMII Media interface selection --------------------------------------*/
#ifdef MII_MODE
#ifdef PHY_CLOCK_MCO
	/* Output HSE clock (25MHz) on MCO pin (PA8) to clock the PHY */
	RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
#endif
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_MII);
	SeNormalPrint(">>>Stm32f407 MAC MII configure...!");
#elif defined RMII_MODE
	/* Output PLL clock divided by 2 (50MHz) on MCO pin (PA8) to clock the PHY */
	RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_2);
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);
	SeNormalPrint(">>>Stm32f407 MAC RMII configure...!");
#endif

	SeNormalPrint(">>>Stm32f407 MAC interface configure...!");

	/* Ethernet pins configuration ************************************************/
	/*

		ETH_MDIO ------------> PA2
		ETH_MDC -------------> PC1
		ETH_RMII_REF_CLK---> PA1
		ETH_RMII_CRS_DV ----> PA7
		ETH_RMII_RXD0 -------> PC4
		ETH_RMII_RXD1 -------> PC5
		ETH_RMII_TX_EN -----> PB11
		ETH_RMII_TXD0 -------> PB12
		ETH_RMII_TXD1 -------> PB13
												  */

	/* Configure PA1, PA2 and PA7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	/* Configure PB11 and PB12  PB13*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_ETH);

	/* Configure PC1,PC4 and PC5 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);

	SeNormalPrint(">>>Stm32f407 MAC I/O configure...!");

	SeDp83848PhyInit();
	SeDp83848PhyReset();

	/*
	 * NVIC configure
	 */
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SeNormalPrint(">>>Stm32f407 MAC interrupt configure...!");

	/*
	 * DMA configure
	 */
	ETH_InitTypeDef ETH_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
						 RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);

	ETH_DeInit();

	ETH_SoftwareReset();

	SeBool blRstTimeout = SeFalse;
	SeInt16 nRstTimes = 100;

	while (ETH_GetSoftwareResetStatus() == SET)
	{
		if(nRstTimes-- < 0)
		{
			blRstTimeout = SeTrue;
			break;
		}
	}

	if(blRstTimeout == SeTrue)
	{
		SeErrorPrint("Stm32f407 MAC wait reset status timeout!");
		return SE_RETURN_ERROR;
	}

	ETH_StructInit(&ETH_InitStructure);

	SeNormalPrint(">>>Stm32f407 MAC software reset...!");

	/*------------------------   MAC   -----------------------------------*/
	ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
	//  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
	//  ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
	//  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;

	ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
	ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
	ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
	ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
	ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
	ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
	ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
	ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
//	#ifdef CHECKSUM_BY_HARDWARE
	ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
//	#endif

	/*------------------------   DMA   -----------------------------------*/
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
	ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
	ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

	ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
	ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
	ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
	ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
	ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

	if(ETH_Init(&ETH_InitStructure, DP83848_PHY_ADDR) != ETH_SUCCESS)
	{
		SeErrorPrint("Stm32f407 ETH initial not absolutely ok!");
	}

	SeNormalPrint(">>>Stm32f407 MAC DMA configure...!");

	ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);

	SeNormalPrint("Stm32f407 MAC configure done!");

	return SE_RETURN_OK;
}
