#include "main.h"

int CSpiModule::ReadWrite(unsigned char* U8Data, int s32Length)
{
   int s32Idx = 0;

#ifdef DEBUG
   Log.StrBlob("SPI TX ", U8Data, s32Length);
#endif

   while(s32Idx < s32Length)
   {
      if((SPI_BLUENRG->SR & SPI_SR_TXE) != 0)
      {
         *(unsigned char*)&SPI_BLUENRG->DR = U8Data[s32Idx];

         while((SPI_BLUENRG->SR & SPI_SR_RXNE) == 0);

         U8Data[s32Idx] = *(unsigned char*)&SPI_BLUENRG->DR;
         s32Idx++;
      }
   }
   while((SPI_BLUENRG->SR & SPI_SR_FTLVL) != 0);

#ifdef DEBUG
   Log.StrBlobR(" RX ", U8Data, s32Length);
#endif

   return 0;
}

void CSpiModule::EnableCS(void)
{
   LL_GPIO_ResetOutputPin(PIN_BLUENRG_SPI_NCS_PORT, PIN_BLUENRG_SPI_NCS_PIN);
}

void CSpiModule::DisableCS(void)
{
   LL_GPIO_SetOutputPin(PIN_BLUENRG_SPI_NCS_PORT, PIN_BLUENRG_SPI_NCS_PIN);
}
