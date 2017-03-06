/*
 * Dp83848_Phy.c
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */
 
#include "Dp83848.h"
#include "../../Manufacturer/stm32f4xx_conf.h"
#include "stm32f4x7_eth.h"

#define SeDP83848PhyReadReg(hAddress)   ETH_ReadPHYRegister(DP83848_PHY_ADDR, hAddress)

void SeDp83848PhyInit(void)
{
	/*
	 * using PD3 as PHY reset pin
	 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void SeDp83848PhyReset(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
	SeDelayCycles(10000);
	GPIO_SetBits(GPIOD, GPIO_Pin_3);
	SeDelayCycles(10000);
}

SeEthernetLinkSpeedType SeDp83848PhyGetLinkSpeed(void)
{
	SeInt32 hReg = 0;
	SeUInt8 bMode = 0;
	SeEthernetLinkSpeedType tLinkType = SeEthernetLinkSpeedTypeUnknown;

	hReg = SeDP83848PhyReadReg(PHY_BCR);

	/* test speed */
	if((hReg & (1<<13)) == (1<<13))
	{
		/* 100MHz*/
		bMode |= 0x01;
	}

	/* test mode */
	if((hReg & (1<<8)) == (1<<8))
	{
		/* full duplex */
		bMode |= 0x02;
	}

	if(bMode == 0x00)
	{
		tLinkType = SeEthernetLinkSpeedType10MHalfDuplex;
	}else if(bMode == 0x01)
	{
		tLinkType = SeEthernetLinkSpeedType100MHalfDuplex;
	}else if(bMode == 0x10)
	{
		tLinkType = SeEthernetLinkSpeedType10MFullDuplex;
	}else if(bMode == 0x11)
	{
		tLinkType = SeEthernetLinkSpeedType100MFullDuplex;
	}

	return tLinkType;
}

SeBool SeDp83848PhyGetIsLinked(void)
{
	if((SeDP83848PhyReadReg(PHY_BSR) & (SeUInt16)0x0004) != (SeUInt16)0x0004)
	{
		return SeFalse;
	}else{
		return SeTrue;
	}
}
