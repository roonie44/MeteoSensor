#include "main.h"

void CI2C::Enable()
{
   LL_I2C_Enable(pI2C);
}
void CI2C::Disable()
{
   LL_I2C_Disable(pI2C);
}

Status CI2C::SetSlaveAddress(unsigned char u8Address)
{
   SlaveAddress = u8Address;
   return Status::OK;
}

Status CI2C::Write(unsigned char *pData, int s32Length)
{
   Log.StrHex("I2C [", SlaveAddress, 1);
   Log.Str("] write ");

   MODIFY_REG  (pI2C->CR2, I2C_CR2_SADD, SlaveAddress << 1);
   MODIFY_REG  (pI2C->CR2, I2C_CR2_NBYTES, s32Length << I2C_CR2_NBYTES_Pos);
   CLEAR_BIT   (pI2C->CR2, I2C_CR2_RD_WRN);  // 0 = write
   SET_BIT     (pI2C->CR2, I2C_CR2_AUTOEND);
   SET_BIT     (pI2C->CR2, I2C_CR2_START);

   while((pI2C->ISR & I2C_ISR_STOPF) == 0)
   {
      if ((pI2C->ISR & I2C_ISR_TXIS) != 0)
      {
         pI2C->TXDR = *pData;
         Log.Hex(*pData, 1);
         pData++;
      }
   }
   SET_BIT(pI2C->ICR, I2C_ICR_STOPCF);

   Log.Str("\r");
   return Status::OK;
}

Status CI2C::Write(unsigned char u8Data)
{
   return Write(&u8Data, 1);
}

Status CI2C::Read(void *pData, int s32Length)
{
   Log.StrHex("I2C [", SlaveAddress, 1);
   Log.Str("] read ");

   MODIFY_REG  (pI2C->CR2, I2C_CR2_SADD, SlaveAddress << 1);
   MODIFY_REG  (pI2C->CR2, I2C_CR2_NBYTES, s32Length << I2C_CR2_NBYTES_Pos);
   SET_BIT     (pI2C->CR2, I2C_CR2_RD_WRN);  // 1 = read
   SET_BIT     (pI2C->CR2, I2C_CR2_AUTOEND);
   SET_BIT     (pI2C->CR2, I2C_CR2_START);

   while((pI2C->ISR & I2C_ISR_STOPF) == 0 || (pI2C->ISR & I2C_ISR_RXNE) != 0)
   {
      if ((pI2C->ISR & I2C_ISR_RXNE) != 0)
      {
         *(unsigned char*)pData = pI2C->RXDR;
         Log.Hex(*(unsigned char*)pData, 1);
         pData = (unsigned char*)pData + 1;
      }
   }
   SET_BIT(pI2C->ICR, I2C_ICR_STOPCF);

   Log.Str("\r");
   return Status::OK;
}
