/*
 * Motor.c
 *
 *  Created on: 2017年3月1日
 *      Author: seeing
 */

#include "Motor.h"
#include "../../Platform/Stm32f107/Stm32f10x.h"

static void SeUserPwmGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}

static void SeUserLeftRollPwmTimerInit()
{
	/*
	 * timer 3
	 */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_DeInit(TIM3);

	/*PCLK1 = 72MHz*/
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 100 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/10000000 - 1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//-----------------------------------------------------------
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;
	//CH1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

static void SeUserRightRollPwmTimerInit()
{
	/*
	 * timer 1
	 */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 100 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/10000000 - 1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	//CH4
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}

SeInt8 SeUserMotorInit(void)
{
	SeUserPwmGpioInit();
	SeUserRightRollPwmTimerInit();
	SeUserLeftRollPwmTimerInit();

	return SE_RETURN_OK;
}

void SeUserMotorSet(SeUserRollStates tLeftDir, SeInt8 nLeftPwmDuty, SeUserRollStates tRightDir, SeInt8 nRightPwmDuty)
{
	SeUInt16 nLeftPeriod, nRightPeriod;
	SeUInt16 nLeftCCR, nRightCCR;

	nLeftPeriod = TIM3->ARR;
	nRightPeriod = TIM1->ARR;

	nLeftCCR = nLeftPwmDuty*nLeftPeriod/100;
	nRightCCR = nRightPwmDuty*nRightPeriod/100;

	if(tLeftDir == SE_USER_ROLL_BACKWARD)
	{
		nLeftCCR = nLeftPeriod - nLeftCCR;
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
	}else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
	}

	if(tRightDir == SE_USER_ROLL_BACKWARD)
	{
		nRightCCR = nRightPeriod - nRightCCR;
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
	}else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	}

	TIM_SetCompare1(TIM3, nLeftCCR);
	TIM_SetCompare4(TIM1, nRightCCR);
}
