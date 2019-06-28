#include "main.h"

Error CI2C::SetSlaveAddress(unsigned char u8Address)
{
   SlaveAddress = u8Address;
   return Error::OK;
}

void CI2C::Write(unsigned char *pData, int s32Length)
{
   Log.StrHex("I2C [", SlaveAddress, 1);
   Log.Str("] write ");

   MODIFY_REG  (pI2C->CR2, I2C_CR2_SADD, SlaveAddress << 1);
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

void CI2C::Write(unsigned char u8Data)
{
   Write(&u8Data, 1);
}

void CI2C::Read(void *pData, int s32Length)
{
   Log.StrHex("I2C [", SlaveAddress, 1);
   Log.Str("] read ");

   MODIFY_REG  (pI2C->CR2, I2C_CR2_SADD, SlaveAddress << 1);
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
         pData = (unsigned char*)pData + 1;
         s32Length--;
      }
   }

   SET_BIT(pI2C->CR2, I2C_CR2_STOP);

   Log.Str("\r");
}
