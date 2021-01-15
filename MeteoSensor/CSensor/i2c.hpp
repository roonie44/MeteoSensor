#pragma once

#include "types.h"

class CI2C
{
   I2C_TypeDef    *pI2C;
   unsigned char  SlaveAddress;

public:
            CI2C              (I2C_TypeDef *pI2C){ this->pI2C = pI2C; }

   void     Enable            ();
   void     Disable           ();
   Status   SetSlaveAddress   (unsigned char u8Address);
   Status   Write             (unsigned char *pData, int s32Length);
   Status   Write             (unsigned char u8Data);
   Status   Read              (void *pData, int s32Length);
};
