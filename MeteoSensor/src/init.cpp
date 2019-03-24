#include "main.h"

#define VECT_TAB_OFFSET  0x00

void Init_SystemClock(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set MSION bit */
  RCC->CR |= RCC_CR_MSION;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON , HSION, and PLLON bits */
  RCC->CR &= (uint32_t)0xEAF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00001000;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

  /* Configure the Vector Table location add offset address ------------------*/
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */

}

void Init_PeriphClock(void)
{
   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
   LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
   LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOH);
   LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
   LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
}

void Init_GPIO(void)
{
   // SWDIO, SWCLK
   //LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_13, LL_GPIO_MODE_ANALOG);
   //LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_14, LL_GPIO_MODE_ANALOG);

   // JTDI, JTDO, NJTRST
   //LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_15, LL_GPIO_MODE_ANALOG);
   //LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_ANALOG);
   //LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);

   // LED
   LL_GPIO_SetPinMode(PIN_LED_GREEN_PORT, PIN_LED_GREEN_PIN, LL_GPIO_MODE_OUTPUT);
   LL_GPIO_ResetOutputPin(PIN_LED_GREEN_PORT, PIN_LED_GREEN_PIN);
}

void Init_LogUSART(void)
{
   LL_USART_InitTypeDef    USART_InitStruct;

   // Pins
   LL_GPIO_SetPinMode(PIN_LOG_USART_PORT, PIN_LOG_USART_TX_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetAFPin_8_15(PIN_LOG_USART_PORT, PIN_LOG_USART_TX_PIN, PIN_LOG_USART_AF);

   LL_GPIO_SetPinMode(PIN_LOG_USART_PORT, PIN_LOG_USART_RX_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetAFPin_8_15(PIN_LOG_USART_PORT, PIN_LOG_USART_RX_PIN, PIN_LOG_USART_AF);

   // USART
   USART_InitStruct.BaudRate              = 115200;
   USART_InitStruct.DataWidth             = LL_USART_DATAWIDTH_8B;
   USART_InitStruct.StopBits              = LL_USART_STOPBITS_1;
   USART_InitStruct.Parity                = LL_USART_PARITY_NONE;
   USART_InitStruct.TransferDirection     = LL_USART_DIRECTION_TX_RX;
   USART_InitStruct.HardwareFlowControl   = LL_USART_HWCONTROL_NONE;
   USART_InitStruct.OverSampling          = LL_USART_OVERSAMPLING_16;

   LL_USART_Init(USART_LOG, &USART_InitStruct);

   LL_USART_EnableDMAReq_TX(USART_LOG);
   LL_USART_EnableDMAReq_RX(USART_LOG);

   // USART Enable
   LL_USART_Enable(USART_LOG);
}

void Init_LogTxDMA(void)
{
   LL_DMA_InitTypeDef    DMA_InitStruct;

   DMA_InitStruct.PeriphOrM2MSrcAddress   = (unsigned int)&USART_LOG->TDR;
   DMA_InitStruct.MemoryOrM2MDstAddress   = 0;
   DMA_InitStruct.Direction               = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
   DMA_InitStruct.Mode                    = LL_DMA_MODE_NORMAL;
   DMA_InitStruct.PeriphOrM2MSrcIncMode   = LL_DMA_PERIPH_NOINCREMENT;
   DMA_InitStruct.MemoryOrM2MDstIncMode   = LL_DMA_MEMORY_INCREMENT;
   DMA_InitStruct.PeriphOrM2MSrcDataSize  = LL_DMA_PDATAALIGN_BYTE;
   DMA_InitStruct.MemoryOrM2MDstDataSize  = LL_DMA_MDATAALIGN_BYTE;
   DMA_InitStruct.NbData                  = 0;
   DMA_InitStruct.PeriphRequest           = DMA_REQUEST_LOG;
   DMA_InitStruct.Priority                = LL_DMA_PRIORITY_MEDIUM;

   LL_DMA_Init(DMA_LOG, DMA_CHANNEL_LOG_TX, &DMA_InitStruct);
}

void Init_LogRxDMA(void)
{
   LL_DMA_InitTypeDef    DMA_InitStruct;

   DMA_InitStruct.PeriphOrM2MSrcAddress   = (unsigned int)&USART_LOG->RDR;
   DMA_InitStruct.MemoryOrM2MDstAddress   = 0;
   DMA_InitStruct.Direction               = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
   DMA_InitStruct.Mode                    = LL_DMA_MODE_NORMAL;
   DMA_InitStruct.PeriphOrM2MSrcIncMode   = LL_DMA_PERIPH_NOINCREMENT;
   DMA_InitStruct.MemoryOrM2MDstIncMode   = LL_DMA_MEMORY_INCREMENT;
   DMA_InitStruct.PeriphOrM2MSrcDataSize  = LL_DMA_PDATAALIGN_BYTE;
   DMA_InitStruct.MemoryOrM2MDstDataSize  = LL_DMA_MDATAALIGN_BYTE;
   DMA_InitStruct.NbData                  = 0;
   DMA_InitStruct.PeriphRequest           = DMA_REQUEST_LOG;
   DMA_InitStruct.Priority                = LL_DMA_PRIORITY_MEDIUM;

   LL_DMA_Init(DMA_LOG, DMA_CHANNEL_LOG_RX, &DMA_InitStruct);
}

void Init_BlueNRG_SPI(void)
{
   LL_SPI_InitTypeDef   SPI_InitStruct;

   // Pins
   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_CLK_PORT, PIN_BLUENRG_SPI_CLK_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetAFPin_0_7(PIN_BLUENRG_SPI_CLK_PORT, PIN_BLUENRG_SPI_CLK_PIN, PIN_BLUENRG_SPI_AF);

   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_MISO_PORT, PIN_BLUENRG_SPI_MISO_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetAFPin_0_7(PIN_BLUENRG_SPI_MISO_PORT, PIN_BLUENRG_SPI_MISO_PIN, PIN_BLUENRG_SPI_AF);

   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_MOSI_PORT, PIN_BLUENRG_SPI_MOSI_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetAFPin_0_7(PIN_BLUENRG_SPI_MOSI_PORT, PIN_BLUENRG_SPI_MOSI_PIN, PIN_BLUENRG_SPI_AF);

   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_RESET_PORT, PIN_BLUENRG_SPI_RESET_PIN, LL_GPIO_MODE_OUTPUT);
   LL_GPIO_ResetOutputPin(PIN_BLUENRG_SPI_RESET_PORT, PIN_BLUENRG_SPI_RESET_PIN);

   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_NCS_PORT, PIN_BLUENRG_SPI_NCS_PIN, LL_GPIO_MODE_OUTPUT);
   LL_GPIO_SetOutputPin(PIN_BLUENRG_SPI_NCS_PORT, PIN_BLUENRG_SPI_NCS_PIN);

   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_IRQ_PORT, PIN_BLUENRG_SPI_IRQ_PIN, LL_GPIO_MODE_INPUT);

   // SPI
   SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
   SPI_InitStruct.Mode              = LL_SPI_MODE_MASTER;
   SPI_InitStruct.DataWidth         = LL_SPI_DATAWIDTH_8BIT;
   SPI_InitStruct.ClockPolarity     = LL_SPI_POLARITY_LOW;
   SPI_InitStruct.ClockPhase        = LL_SPI_PHASE_1EDGE;
   SPI_InitStruct.NSS               = LL_SPI_NSS_SOFT;
   SPI_InitStruct.BaudRate          = LL_SPI_BAUDRATEPRESCALER_DIV2;
   SPI_InitStruct.BitOrder          = LL_SPI_MSB_FIRST;
   SPI_InitStruct.CRCCalculation    = LL_SPI_CRCCALCULATION_DISABLE;
   SPI_InitStruct.CRCPoly           = 0;

   LL_SPI_Init(SPI_BLUENRG, &SPI_InitStruct);

   LL_SPI_SetRxFIFOThreshold(SPI_BLUENRG, LL_SPI_RX_FIFO_TH_QUARTER);

   LL_SPI_Enable(SPI_BLUENRG);
}

void Init_I2C(void)
{
   LL_GPIO_SetAFPin_8_15   (PIN_SENSOR_I2C_SCL_PORT, PIN_SENSOR_I2C_SCL_PIN, PIN_SENSOR_I2C_SCL_AF);
   LL_GPIO_SetPinMode      (PIN_SENSOR_I2C_SCL_PORT, PIN_SENSOR_I2C_SCL_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinOutputType(PIN_SENSOR_I2C_SCL_PORT, PIN_SENSOR_I2C_SCL_PIN, LL_GPIO_OUTPUT_OPENDRAIN);
   LL_GPIO_SetPinSpeed     (PIN_SENSOR_I2C_SCL_PORT, PIN_SENSOR_I2C_SCL_PIN, LL_GPIO_SPEED_HIGH);

   LL_GPIO_SetAFPin_8_15   (PIN_SENSOR_I2C_SDA_PORT, PIN_SENSOR_I2C_SDA_PIN, PIN_SENSOR_I2C_SDA_AF);
   LL_GPIO_SetPinMode      (PIN_SENSOR_I2C_SDA_PORT, PIN_SENSOR_I2C_SDA_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinOutputType(PIN_SENSOR_I2C_SDA_PORT, PIN_SENSOR_I2C_SDA_PIN, LL_GPIO_OUTPUT_OPENDRAIN);
   LL_GPIO_SetPinSpeed     (PIN_SENSOR_I2C_SDA_PORT, PIN_SENSOR_I2C_SDA_PIN, LL_GPIO_SPEED_HIGH);

   LL_GPIO_SetPinMode(PIN_HTS221_I2C_DRDY_PORT, PIN_HTS221_I2C_DRDY_PIN, LL_GPIO_MODE_ANALOG); // Unused
   LL_GPIO_SetPinMode(PIN_LPS22HB_I2C_INT_PORT, PIN_LPS22HB_I2C_INT_PIN, LL_GPIO_MODE_ANALOG); // Unused

   LL_I2C_Disable(I2C_SENSOR);

   MODIFY_REG  (I2C_SENSOR->TIMINGR, I2C_TIMINGR_PRESC,  0);
   MODIFY_REG  (I2C_SENSOR->TIMINGR, I2C_TIMINGR_SCLL,   0x09 << I2C_TIMINGR_SCLL_Pos);
   MODIFY_REG  (I2C_SENSOR->TIMINGR, I2C_TIMINGR_SCLH,   0x03 << I2C_TIMINGR_SCLH_Pos);
   MODIFY_REG  (I2C_SENSOR->TIMINGR, I2C_TIMINGR_SDADEL, 0x01 << I2C_TIMINGR_SDADEL_Pos);
   MODIFY_REG  (I2C_SENSOR->TIMINGR, I2C_TIMINGR_SCLDEL, 0x01 << I2C_TIMINGR_SCLDEL_Pos);
   SET_BIT     (I2C_SENSOR->CR1,     I2C_CR1_ANFOFF);

   LL_I2C_Enable(I2C_SENSOR);
}

void Init_Interrupts(void)
{
   // Log TX Transfer Complete
   LL_DMA_EnableIT_TC(DMA_LOG, DMA_CHANNEL_LOG_TX);
   NVIC_EnableIRQ(DMA1_Channel2_IRQn);

   // Log RX Line Idle
   LL_USART_ClearFlag_IDLE(USART_LOG);
   LL_USART_EnableIT_IDLE(USART_LOG);
   NVIC_EnableIRQ(USART3_IRQn);

   // BlueNRG IRQ
   LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);
   LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
   LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
   LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
   NVIC_EnableIRQ(EXTI0_IRQn);
}

void Init_Hardware(void)
{
   Init_SystemClock();
   Init_PeriphClock();
   Init_GPIO();

   Init_LogUSART();
   Init_LogTxDMA();
   Init_LogRxDMA();

   Init_BlueNRG_SPI();
   Init_I2C();

   Init_Interrupts();
}

void Init_Stop(void)
{
   // BlueNRG SPI
   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_CLK_PORT, PIN_BLUENRG_SPI_CLK_PIN, LL_GPIO_MODE_ANALOG);
   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_MISO_PORT, PIN_BLUENRG_SPI_MISO_PIN, LL_GPIO_MODE_ANALOG);
   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_MOSI_PORT, PIN_BLUENRG_SPI_MOSI_PIN, LL_GPIO_MODE_ANALOG);

   // Log USART
   LL_GPIO_SetPinMode(PIN_LOG_USART_PORT, PIN_LOG_USART_TX_PIN, LL_GPIO_MODE_ANALOG);
   LL_GPIO_SetPinMode(PIN_LOG_USART_PORT, PIN_LOG_USART_RX_PIN, LL_GPIO_MODE_ANALOG);
}

void Init_Wakeup(void)
{
   // BlueNRG SPI
   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_CLK_PORT, PIN_BLUENRG_SPI_CLK_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_MISO_PORT, PIN_BLUENRG_SPI_MISO_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinMode(PIN_BLUENRG_SPI_MOSI_PORT, PIN_BLUENRG_SPI_MOSI_PIN, LL_GPIO_MODE_ALTERNATE);

   // Log USART
   LL_GPIO_SetPinMode(PIN_LOG_USART_PORT, PIN_LOG_USART_TX_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinMode(PIN_LOG_USART_PORT, PIN_LOG_USART_RX_PIN, LL_GPIO_MODE_ALTERNATE);
}
