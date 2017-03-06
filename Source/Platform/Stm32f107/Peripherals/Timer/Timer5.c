/*
 * Timer5.c
 *
 *  Created on: 2017年2月28日
 *      Author: seeing
 */

#include "Timer5.h"
#include "../../Manufacturer/stm32f10x_conf.h"

static SeTimerCallback tGlobalCallback;

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

		if(tGlobalCallback.fpSeTimerCallback != SeNull)
			tGlobalCallback.fpSeTimerCallback();

	}
}

SeInt8 SeStm32f107Timer5Init(SeTimeValue tInterval, SeTimerCallback tCallback)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if(tCallback.fpSeTimerCallback == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(tInterval.iUtcSeconds == 0 && tInterval.iMicroSeconds < 1000)
	{
		SeErrorPrint("Stm32f107 don't accept this interval!");
		return SE_RETURN_ERROR;
	}

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_DeInit(TIM5);

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if(tInterval.iUtcSeconds > 0)
	{
		TIM_TimeBaseStructure.TIM_Period = (SystemCoreClock*(tInterval.iUtcSeconds + tInterval.iMicroSeconds/1000000))/(SystemCoreClock/1000) - 1;
		TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000 - 1);
	}else{
		TIM_TimeBaseStructure.TIM_Period = (SystemCoreClock*(tInterval.iMicroSeconds/1000000))/(SystemCoreClock/1000000) - 1;
		TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000000 - 1);
	}

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM5, DISABLE);

	tGlobalCallback = tCallback;

	return SE_RETURN_OK;
}

void SeStm32f107Timer5Start(void)
{
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
}

void SeStm32f107Timer5Stop(void)
{
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM5, DISABLE);
}
