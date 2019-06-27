#include "main.h"

void CI2CModule::Write(unsigned int u32SlaveAddress, unsigned char *pData, int s32Length)
{
   Log.StrHex("I2C [", u32SlaveAddress, 1);
   Log.Str("] write ");

   MODIFY_REG  (pI2C->CR2, I2C_CR2_SADD, u32SlaveAddress << 1);
   MODIFY_REG  (pI2C->CR2, I2C_CR2_NBYTES, s32Length << I2C_CR2_NBYTES_Pos);
   CLEAR_BIT   (pI2C->CR2, I2C_CR2_RD_WRN);  // 0 = write
   //SET_BIT     (I2C_SENSOR->CR2, I2C_CR2_AUTOEND);
   SET_BIT     (pI2C->CR2, I2C_CR2_START);

   do
   {
      if ((pI2C->ISR & I2C_ISR_TXIS) != 0)
      {
         pI2C->TXDR = *pData;
         Log.Hex(*pData, 1);
         pData++;
      }

      if((pI2C->ISR & I2C_ISR_NACKF) != 0)
      {
         Log.Str(" nack err\r");
         return;
      }
   }
   while ((pI2C->ISR & I2C_ISR_TC) == 0);

   SET_BIT(pI2C->CR2, I2C_CR2_STOP);

   Log.Str("\r");
}

void CI2CModule::Write(unsigned int u32SlaveAddress, unsigned char u8Data)
{
   Write(u32SlaveAddress, &u8Data, 1);
}

void CI2CModule::Read(unsigned int u32SlaveAddress, void *pData, int s32Length)
{
   Log.StrHex("I2C [", u32SlaveAddress, 1);
   Log.Str("] read ");

   MODIFY_REG  (pI2C->CR2, I2C_CR2_SADD, u32SlaveAddress << 1);
   MODIFY_REG  (pI2C->CR2, I2C_CR2_NBYTES, s32Length << I2C_CR2_NBYTES_Pos);
   SET_BIT     (pI2C->CR2, I2C_CR2_RD_WRN);  // 1 = read
   //SET_BIT     (I2C_SENSOR->CR2, I2C_CR2_AUTOEND);
   SET_BIT     (pI2C->CR2, I2C_CR2_START);

   while (s32Length > 0)
   {
      if ((pI2C->ISR & I2C_ISR_RXNE) != 0)
      {
         *(unsigned char*)pData = pI2C->RXDR;
         Log.Hex(*(unsigned char*)pData, 1);
         pData++;
         s32Length--;
      }
   }

   SET_BIT(pI2C->CR2, I2C_CR2_STOP);

   Log.Str("\r");
}
