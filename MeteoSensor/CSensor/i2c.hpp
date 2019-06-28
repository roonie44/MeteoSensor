#ifndef I2C_CPP_
#define I2C_CPP_
#include "types.h"

class CI2C
{
   I2C_TypeDef    *pI2C;
   unsigned char  SlaveAddress;

public:
         CI2C              (I2C_TypeDef *pI2C){ this->pI2C = pI2C; }

   Error SetSlaveAddress   (unsigned char u8Address);
   void  Write             (unsigned char *pData, int s32Length);
   void  Write             (unsigned char u8Data);
   void  Read              (void *pData, int s32Length);
};



#endif /* I2C_CPP_ */
