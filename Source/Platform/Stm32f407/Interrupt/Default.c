/*
 * Default.c
 *
 *  Created on: 2016年2月25日
 *      Author: seeing
 */

#include "Default.h"

void SeStm32f407DefaultIsr(void)
{
	SeErrorPrint("Please redefine Isr!\r\n");
	while(1)
	{
	}
}

void WWDG_IRQHandler(void){SeStm32f407DefaultIsr();}
void PVD_IRQHandler(void){SeStm32f407DefaultIsr();}
void TAMP_STAMP_IRQHandler(void){SeStm32f407DefaultIsr();}
void RTC_WKUP_IRQHandler(void){SeStm32f407DefaultIsr();}
void FLASH_IRQHandler(void){SeStm32f407DefaultIsr();}
void RCC_IRQHandler(void){SeStm32f407DefaultIsr();}
void EXTI0_IRQHandler(void){SeStm32f407DefaultIsr();}
void EXTI1_IRQHandler(void){SeStm32f407DefaultIsr();}
void EXTI2_IRQHandler(void){SeStm32f407DefaultIsr();}
void EXTI3_IRQHandler(void){SeStm32f407DefaultIsr();}
void EXTI4_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA1_Stream0_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA1_Stream1_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA1_Stream2_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA1_Stream3_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA1_Stream4_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA1_Stream5_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA1_Stream6_IRQHandler(void){SeStm32f407DefaultIsr();}
void ADC_IRQHandler(void){SeStm32f407DefaultIsr();}
void CAN1_TX_IRQHandler(void){SeStm32f407DefaultIsr();}
void CAN1_RX0_IRQHandler(void){SeStm32f407DefaultIsr();}
void CAN1_RX1_IRQHandler(void){SeStm32f407DefaultIsr();}
void CAN1_SCE_IRQHandler(void){SeStm32f407DefaultIsr();}
void EXTI9_5_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM1_BRK_TIM9_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM1_UP_TIM10_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM1_TRG_COM_TIM11_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM1_CC_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM2_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM3_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM4_IRQHandler(void){SeStm32f407DefaultIsr();}
void I2C1_EV_IRQHandler(void){SeStm32f407DefaultIsr();}
void I2C1_ER_IRQHandler(void){SeStm32f407DefaultIsr();}
void I2C2_EV_IRQHandler(void){SeStm32f407DefaultIsr();}
void I2C2_ER_IRQHandler(void){SeStm32f407DefaultIsr();}
void SPI1_IRQHandler(void){SeStm32f407DefaultIsr();}
void SPI2_IRQHandler(void){SeStm32f407DefaultIsr();}
void USART2_IRQHandler(void){SeStm32f407DefaultIsr();}
void USART3_IRQHandler(void){SeStm32f407DefaultIsr();}
void EXTI15_10_IRQHandler(void){SeStm32f407DefaultIsr();}
void RTC_Alarm_IRQHandler(void){SeStm32f407DefaultIsr();}
void OTG_FS_WKUP_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM8_BRK_TIM12_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM8_UP_TIM13_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM8_TRG_COM_TIM14_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM8_CC_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA1_Stream7_IRQHandler(void){SeStm32f407DefaultIsr();}
void FSMC_IRQHandler(void){SeStm32f407DefaultIsr();}
void SDIO_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM5_IRQHandler(void){SeStm32f407DefaultIsr();}
void SPI3_IRQHandler(void){SeStm32f407DefaultIsr();}
void UART4_IRQHandler(void){SeStm32f407DefaultIsr();}
void UART5_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM6_DAC_IRQHandler(void){SeStm32f407DefaultIsr();}
void TIM7_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA2_Stream0_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA2_Stream1_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA2_Stream2_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA2_Stream3_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA2_Stream4_IRQHandler(void){SeStm32f407DefaultIsr();}
void ETH_WKUP_IRQHandler(void){SeStm32f407DefaultIsr();}
void CAN2_TX_IRQHandler(void){SeStm32f407DefaultIsr();}
void CAN2_RX0_IRQHandler(void){SeStm32f407DefaultIsr();}
void CAN2_RX1_IRQHandler(void){SeStm32f407DefaultIsr();}
void CAN2_SCE_IRQHandler(void){SeStm32f407DefaultIsr();}
void OTG_FS_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA2_Stream5_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA2_Stream6_IRQHandler(void){SeStm32f407DefaultIsr();}
void DMA2_Stream7_IRQHandler(void){SeStm32f407DefaultIsr();}
void USART6_IRQHandler(void){SeStm32f407DefaultIsr();}
void I2C3_EV_IRQHandler(void){SeStm32f407DefaultIsr();}
void I2C3_ER_IRQHandler(void){SeStm32f407DefaultIsr();}
void OTG_HS_EP1_OUT_IRQHandler(void){SeStm32f407DefaultIsr();}
void OTG_HS_EP1_IN_IRQHandler(void){SeStm32f407DefaultIsr();}
void OTG_HS_WKUP_IRQHandler(void){SeStm32f407DefaultIsr();}
void OTG_HS_IRQHandler(void){SeStm32f407DefaultIsr();}
void DCMI_IRQHandler(void){SeStm32f407DefaultIsr();}
void CRYP_IRQHandler(void){SeStm32f407DefaultIsr();}
void HASH_RNG_IRQHandler(void){SeStm32f407DefaultIsr();}
void FPU_IRQHandler(void){SeStm32f407DefaultIsr();}
