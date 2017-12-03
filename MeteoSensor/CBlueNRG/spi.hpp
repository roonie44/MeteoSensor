#ifndef SPI_HPP_
#define SPI_HPP_

class CSpiModule
{
public:
   int            ReadWrite      (unsigned char* U8Data, int s32Length);
   void           EnableCS       (void);
   void           DisableCS      (void);
};

#endif /* SPI_HPP_ */
