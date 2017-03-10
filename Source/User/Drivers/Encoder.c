/*
 * Encoder.c
 *
 *  Created on: 2017年3月1日
 *      Author: seeing
 */

#include "Encoder.h"
#include "../../Platform/Stm32f107/Stm32f10x.h"

static SeUserEncoderOperation tGlobalEncoderOperation;

void TIM4_IRQHandler(void)
{
	TIM4->CNT = 0;

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		/*
		 * when speed is too slow
		 */
		if(tGlobalEncoderOperation.fpSeUserEncoderSteady)
			tGlobalEncoderOperation.fpSeUserEncoderSteady();
		TIM_ClearFlag(TIM4, TIM_IT_CC3|TIM_IT_CC4|TIM_FLAG_Update);
	}

	//CH3
    if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
	{
    	/*
    	 * right roll trigger
    	 */
    	if(tGlobalEncoderOperation.fpSeUserRightEncoderTrigger)
    		tGlobalEncoderOperation.fpSeUserRightEncoderTrigger();
	}

	//CH4
    if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
	{
    	/*
    	 * left roll trigger
    	 */
    	if(tGlobalEncoderOperation.fpSeUserLeftEncoderTrigger)
    	    tGlobalEncoderOperation.fpSeUserLeftEncoderTrigger();
	}
	TIM_ClearFlag(TIM4, TIM_IT_CC3|TIM_IT_CC4|TIM_FLAG_Update);
}

SeInt8 SeUserEncoderInit(SeUserEncoderOperation tUserEncoderOperation)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	if(tUserEncoderOperation.fpSeUserLeftEncoderTrigger == SeNull || tUserEncoderOperation.fpSeUserRightEncoderTrigger == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	tGlobalEncoderOperation = tUserEncoderOperation;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	TIM_DeInit(TIM4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_9);

	//TIM4 Capture Compare Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//PCLK1 = 72MHz
	//Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 10000;
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000 - 1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM4, TIM_IT_CC3|TIM_IT_CC4|TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM4, ENABLE);

	return SE_RETURN_OK;
}
