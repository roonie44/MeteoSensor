#include "main.h"

void CI2CModule::Write(unsigned int u32SlaveAddress, unsigned char *pData, int s32Length)
{
   Log.Str("I2C write ");

   MODIFY_REG  (I2C_SENSOR->CR2, I2C_CR2_SADD, I2C_ADDRESS_HTS221 << 1);
   MODIFY_REG  (I2C_SENSOR->CR2, I2C_CR2_NBYTES, s32Length << I2C_CR2_NBYTES_Pos);
   CLEAR_BIT   (I2C_SENSOR->CR2, I2C_CR2_RD_WRN);  // 0 = write
   //SET_BIT     (I2C_SENSOR->CR2, I2C_CR2_AUTOEND);
   SET_BIT     (I2C_SENSOR->CR2, I2C_CR2_START);

   do
   {
      if ((I2C_SENSOR->ISR & I2C_ISR_TXIS) != 0)
      {
         I2C_SENSOR->TXDR = *pData;
         Log.Hex(*pData, 1);
         pData++;
      }

      if((I2C_SENSOR->ISR & I2C_ISR_NACKF) != 0)
      {
         Log.Str(" nack err\r");
         return;
      }
   }
   while ((I2C_SENSOR->ISR & I2C_ISR_TC) == 0);

   SET_BIT(I2C_SENSOR->CR2, I2C_CR2_STOP);

   Log.Str("\r");
}

void CI2CModule::Write(unsigned int u32SlaveAddress, unsigned char u8Data)
{
   Write(u32SlaveAddress, &u8Data, 1);
}

void CI2CModule::Read(unsigned int u32SlaveAddress, void *pData, int s32Length)
{
   Log.Str("I2C read ");

   MODIFY_REG  (I2C_SENSOR->CR2, I2C_CR2_SADD, I2C_ADDRESS_HTS221 << 1);
   MODIFY_REG  (I2C_SENSOR->CR2, I2C_CR2_NBYTES, s32Length << I2C_CR2_NBYTES_Pos);
   SET_BIT     (I2C_SENSOR->CR2, I2C_CR2_RD_WRN);  // 1 = read
   //SET_BIT     (I2C_SENSOR->CR2, I2C_CR2_AUTOEND);
   SET_BIT     (I2C_SENSOR->CR2, I2C_CR2_START);

   while (s32Length > 0)
   {
      if ((I2C_SENSOR->ISR & I2C_ISR_RXNE) != 0)
      {
         *(unsigned char*)pData = I2C_SENSOR->RXDR;
         Log.Hex(*(unsigned char*)pData, 1);
         pData++;
         s32Length--;
      }
   }

   SET_BIT(I2C_SENSOR->CR2, I2C_CR2_STOP);

   Log.Str("\r");
}
