/*
 * Default.c
 *
 *  Created on: 2016年2月25日
 *      Author: seeing
 */

#include "Default.h"

void SeStm32f107DefaultIsr(void)
{
	SeErrorPrint("Please redefine Isr!\r\n");
	while(1)
	{
	}
}

void WWDG_IRQHandler(void){SeStm32f107DefaultIsr();}
void PVD_IRQHandler(void){SeStm32f107DefaultIsr();}
void TAMPER_IRQHandler(void){SeStm32f107DefaultIsr();}
void RTC_IRQHandler(void){SeStm32f107DefaultIsr();}
void FLASH_IRQHandler(void){SeStm32f107DefaultIsr();}
void RCC_IRQHandler(void){SeStm32f107DefaultIsr();}
void EXTI0_IRQHandler(void){SeStm32f107DefaultIsr();}
void EXTI1_IRQHandler(void){SeStm32f107DefaultIsr();}
void EXTI2_IRQHandler(void){SeStm32f107DefaultIsr();}
void EXTI3_IRQHandler(void){SeStm32f107DefaultIsr();}
void EXTI4_IRQHandler(void){SeStm32f107DefaultIsr();}
void DMA1_Channel1_IRQHandler(void){SeStm32f107DefaultIsr();}
void DMA1_Channel2_IRQHandler(void){SeStm32f107DefaultIsr();}
void DMA1_Channel3_IRQHandler(void){SeStm32f107DefaultIsr();}
//void DMA1_Channel4_IRQHandler(void){SeStm32f107DefaultIsr();}
//void DMA1_Channel5_IRQHandler(void){SeStm32f107DefaultIsr();}
void DMA1_Channel6_IRQHandler(void){SeStm32f107DefaultIsr();}
void DMA1_Channel7_IRQHandler(void){SeStm32f107DefaultIsr();}
void ADC1_2_IRQHandler(void){SeStm32f107DefaultIsr();}
void USB_HP_CAN1_TX_IRQHandler(void){SeStm32f107DefaultIsr();}
void USB_LP_CAN1_RX0_IRQHandler(void){SeStm32f107DefaultIsr();}
void CAN1_RX1_IRQHandler(void){SeStm32f107DefaultIsr();}
void CAN1_SCE_IRQHandler(void){SeStm32f107DefaultIsr();}
void EXTI9_5_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM1_BRK_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM1_UP_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM1_TRG_COM_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM1_CC_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM2_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM3_IRQHandler(void){SeStm32f107DefaultIsr();}
//void TIM4_IRQHandler(void){SeStm32f107DefaultIsr();}
void I2C1_EV_IRQHandler(void){SeStm32f107DefaultIsr();}
void I2C1_ER_IRQHandler(void){SeStm32f107DefaultIsr();}
void I2C2_EV_IRQHandler(void){SeStm32f107DefaultIsr();}
void I2C2_ER_IRQHandler(void){SeStm32f107DefaultIsr();}
void SPI1_IRQHandler(void){SeStm32f107DefaultIsr();}
void SPI2_IRQHandler(void){SeStm32f107DefaultIsr();}
//void USART1_IRQHandler(void){SeStm32f107DefaultIsr();}
void USART2_IRQHandler(void){SeStm32f107DefaultIsr();}
void USART3_IRQHandler(void){SeStm32f107DefaultIsr();}
void EXTI15_10_IRQHandler(void){SeStm32f107DefaultIsr();}
void RTCAlarm_IRQHandler(void){SeStm32f107DefaultIsr();}
void USBWakeUp_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM8_BRK_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM8_UP_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM8_TRG_COM_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM8_CC_IRQHandler(void){SeStm32f107DefaultIsr();}
void ADC3_IRQHandler(void){SeStm32f107DefaultIsr();}
void FSMC_IRQHandler(void){SeStm32f107DefaultIsr();}
void SDIO_IRQHandler(void){SeStm32f107DefaultIsr();}
//void TIM5_IRQHandler(void){SeStm32f107DefaultIsr();}
void SPI3_IRQHandler(void){SeStm32f107DefaultIsr();}
void UART4_IRQHandler(void){SeStm32f107DefaultIsr();}
void UART5_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM6_IRQHandler(void){SeStm32f107DefaultIsr();}
void TIM7_IRQHandler(void){SeStm32f107DefaultIsr();}
void DMA2_Channel1_IRQHandler(void){SeStm32f107DefaultIsr();}
void DMA2_Channel2_IRQHandler(void){SeStm32f107DefaultIsr();}
void DMA2_Channel3_IRQHandler(void){SeStm32f107DefaultIsr();}
void DMA2_Channel4_5_IRQHandler(void){SeStm32f107DefaultIsr();}
