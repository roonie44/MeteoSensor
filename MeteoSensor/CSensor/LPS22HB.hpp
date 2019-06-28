#ifndef LPS22HB_HPP_
#define LPS22HB_HPP_

class CLPS22HB
{
public:
   void Init   (void);

private:
   static const unsigned char REG_ADDR_LPS22HB_WHO_AM_I     = 0x0F;
   static const unsigned char REG_VAL_LPS22HB_WHO_AM_I      = 0xB1;
   static const unsigned char REG_ADDR_LPS22HB_CTRL1        = 0x10;
   static const unsigned char REG_ADDR_LPS22HB_CTRL2        = 0x11;
   static const unsigned char REG_ADDR_LPS22HB_STATUS       = 0x27;
   static const unsigned char REG_ADDR_LPS22HB_PRESS_OUT_XL = 0x28;

   void           CheckState            (void);
   void           NewMeasurement        (void);
   void           ReadPressure          (void);
};

#endif /* LPS22HB_HPP_ */
