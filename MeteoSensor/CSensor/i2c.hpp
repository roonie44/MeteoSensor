#ifndef I2C_CPP_
#define I2C_CPP_

class CI2CModule
{
public:
   I2C_TypeDef *pI2C;

public:
   CI2CModule  (I2C_TypeDef *pI2C){ this->pI2C = pI2C; }

   void Write  (unsigned int u32SlaveAddress, unsigned char *pData, int s32Length);
   void Write  (unsigned int u32SlaveAddress, unsigned char u8Data);
   void Read   (unsigned int u32SlaveAddress, void *pData, int s32Length);
};



#endif /* I2C_CPP_ */
