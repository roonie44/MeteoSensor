#ifndef I2C_CPP_
#define I2C_CPP_

class CI2CModule
{
public:
   void Write  (unsigned int u32SlaveAddress, unsigned char *pData, int s32Length);
   void Write  (unsigned int u32SlaveAddress, unsigned char u8Data);
   void Read   (unsigned int u32SlaveAddress, void *pData, int s32Length);
};



#endif /* I2C_CPP_ */
