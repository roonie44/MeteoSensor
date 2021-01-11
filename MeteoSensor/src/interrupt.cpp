#include "main.h"

extern void *_sidata, *_sdata, *_edata, *_sbss, *_ebss, *_estack;
extern "C" {
void __libc_init_array(void);
void Reset_Handler(void);
}

void SysTick_Handler(void)
{
   CClock::Tick();
}

void RTC_WKUP_IRQHandler(void)
{
   LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_20);

   LL_RTC_DisableWriteProtection(RTC);
   LL_RTC_ClearFlag_WUT(RTC);
   LL_RTC_EnableWriteProtection(RTC);
}

void DMA1_Channel2_IRQHandler(void)
{
   unsigned int ISR = DMA1->ISR;

   if(ISR & DMA_ISR_TCIF2)
   {
      DMA1->IFCR |= DMA_IFCR_CTCIF2;
      Log.HandleTx();
   }
}

void USART3_IRQHandler(void)
{
   unsigned int ISR = USART3->ISR;

   if(ISR & USART_ISR_IDLE)
   {
      USART3->ICR |= USART_ICR_IDLECF;
      Log.HandleRx();
   }

}

void EXTI0_IRQHandler(void)
{
   EXTI->PR1 = EXTI_PR1_PIF0;
   BlueNRG.HandleIRQ();
}

void Default_Handler(void)
{
   while(1);
}

void* g_pfnVectors[] __attribute__ ((section (".isr_vector"))) =
{
   &_estack,
   (void*)&Reset_Handler,
   (void*)&Default_Handler,            // NMI_Handler,
   (void*)&Default_Handler,            // HardFault_Handler,
   (void*)&Default_Handler,            // MemManage_Handler,
   (void*)&Default_Handler,            // BusFault_Handler,
   (void*)&Default_Handler,            // UsageFault_Handler,
   0,
   0,
   0,
   0,
   (void*)&Default_Handler,            // SVC_Handler,
   (void*)&Default_Handler,            // DebugMon_Handler,
   0,
   (void*)&Default_Handler,            // PendSV_Handler,
   (void*)&SysTick_Handler,
   (void*)&Default_Handler,            // WWDG_IRQHandler,
   (void*)&Default_Handler,            // PVD_PVM_IRQHandler,
   (void*)&Default_Handler,            // TAMP_STAMP_IRQHandler,
   (void*)&RTC_WKUP_IRQHandler,
   (void*)&Default_Handler,            // FLASH_IRQHandler,
   (void*)&Default_Handler,            // RCC_IRQHandler,
   (void*)&EXTI0_IRQHandler,
   (void*)&Default_Handler,            // EXTI1_IRQHandler,
   (void*)&Default_Handler,            // EXTI2_IRQHandler,
   (void*)&Default_Handler,            // EXTI3_IRQHandler,
   (void*)&Default_Handler,            // EXTI4_IRQHandler,
   (void*)&Default_Handler,            // DMA1_Channel1_IRQHandler,
   (void*)&DMA1_Channel2_IRQHandler,
   (void*)&Default_Handler,            // DMA1_Channel3_IRQHandler,
   (void*)&Default_Handler,            // DMA1_Channel4_IRQHandler,
   (void*)&Default_Handler,            // DMA1_Channel5_IRQHandler,
   (void*)&Default_Handler,            // DMA1_Channel6_IRQHandler,
   (void*)&Default_Handler,            // DMA1_Channel7_IRQHandler,
   (void*)&Default_Handler,            // ADC1_IRQHandler,
   (void*)&Default_Handler,            // CAN1_TX_IRQHandler,
   (void*)&Default_Handler,            // CAN1_RX0_IRQHandler,
   (void*)&Default_Handler,            // CAN1_RX1_IRQHandler,
   (void*)&Default_Handler,            // CAN1_SCE_IRQHandler,
   (void*)&Default_Handler,            // EXTI9_5_IRQHandler,
   (void*)&Default_Handler,            // TIM1_BRK_TIM15_IRQHandler,
   (void*)&Default_Handler,            // TIM1_UP_TIM16_IRQHandler,
   (void*)&Default_Handler,            // TIM1_TRG_COM_IRQHandler,
   (void*)&Default_Handler,            // TIM1_CC_IRQHandler,
   (void*)&Default_Handler,            // TIM2_IRQHandler,
   (void*)&Default_Handler,            // TIM3_IRQHandler,
   (void*)&Default_Handler,            // TIM4_IRQHandler,
   (void*)&Default_Handler,            // I2C1_EV_IRQHandler,
   (void*)&Default_Handler,            // I2C1_ER_IRQHandler,
   (void*)&Default_Handler,            // I2C2_EV_IRQHandler,
   (void*)&Default_Handler,            // I2C2_ER_IRQHandler,
   (void*)&Default_Handler,            // SPI1_IRQHandler,
   (void*)&Default_Handler,            // SPI2_IRQHandler,
   (void*)&Default_Handler,            // USART1_IRQHandler,
   (void*)&Default_Handler,            // USART2_IRQHandler,
   (void*)&USART3_IRQHandler,
   (void*)&Default_Handler,            // EXTI15_10_IRQHandler,
   (void*)&Default_Handler,            // RTC_Alarm_IRQHandler,
   (void*)&Default_Handler,            // DFSDM3_IRQHandler,
   (void*)&Default_Handler,            // TIM8_BRK_IRQHandler,
   (void*)&Default_Handler,            // TIM8_UP_IRQHandler,
   (void*)&Default_Handler,            // TIM8_TRG_COM_IRQHandler,
   (void*)&Default_Handler,            // TIM8_CC_IRQHandler,
   (void*)&Default_Handler,            // ADC3_IRQHandler,
   (void*)&Default_Handler,            // FMC_IRQHandler,
   (void*)&Default_Handler,            // SDMMC1_IRQHandler,
   (void*)&Default_Handler,            // TIM5_IRQHandler,
   (void*)&Default_Handler,            // SPI3_IRQHandler,
   (void*)&Default_Handler,            // UART4_IRQHandler,
   (void*)&Default_Handler,            // UART5_IRQHandler,
   (void*)&Default_Handler,            // TIM6_DAC_IRQHandler,
   (void*)&Default_Handler,            // TIM7_IRQHandler,
   (void*)&Default_Handler,            // DMA2_Channel1_IRQHandler,
   (void*)&Default_Handler,            // DMA2_Channel2_IRQHandler,
   (void*)&Default_Handler,            // DMA2_Channel3_IRQHandler,
   (void*)&Default_Handler,            // DMA2_Channel4_IRQHandler,
   (void*)&Default_Handler,            // DMA2_Channel5_IRQHandler,
   (void*)&Default_Handler,            // DFSDM0_IRQHandler,
   (void*)&Default_Handler,            // DFSDM1_IRQHandler,
   (void*)&Default_Handler,            // DFSDM2_IRQHandler,
   (void*)&Default_Handler,            // COMP_IRQHandler,
   (void*)&Default_Handler,            // LPTIM1_IRQHandler,
   (void*)&Default_Handler,            // LPTIM2_IRQHandler,
   (void*)&Default_Handler,            // OTG_FS_IRQHandler,
   (void*)&Default_Handler,            // DMA2_Channel6_IRQHandler,
   (void*)&Default_Handler,            // DMA2_Channel7_IRQHandler,
   (void*)&Default_Handler,            // LPUART1_IRQHandler,
   (void*)&Default_Handler,            // QUADSPI_IRQHandler,
   (void*)&Default_Handler,            // I2C3_EV_IRQHandler,
   (void*)&Default_Handler,            // I2C3_ER_IRQHandler,
   (void*)&Default_Handler,            // SAI1_IRQHandler,
   (void*)&Default_Handler,            // SAI2_IRQHandler,
   (void*)&Default_Handler,            // SWPMI1_IRQHandler,
   (void*)&Default_Handler,            // TSC_IRQHandler,
   (void*)&Default_Handler,            // LCD_IRQHandler,
   0,
   (void*)&Default_Handler,            // RNG_IRQHandler,
   (void*)&Default_Handler,            // FPU_IRQHandler,
};


extern "C" {
//void __attribute__((naked, noreturn)) Reset_Handler()
void Reset_Handler(void)
{
   SCB->VTOR = (uint32_t)&g_pfnVectors;
   //Normally the CPU will setup the based on the value from the first entry in the vector table.
   //If you encounter problems with accessing stack variables during initialization, ensure
   asm ("ldr sp, =_estack");

   void **pSource, **pDest;

   // Copy the data segment initializers from flash to SRAM
   for (pSource = &_sidata, pDest = &_sdata; pDest != &_edata; pSource++, pDest++)
      *pDest = *pSource;

   // Zero fill the bss segment
   for (pDest = &_sbss; pDest != &_ebss; pDest++)
      *pDest = 0;

   __libc_init_array();

   main();
}
}
