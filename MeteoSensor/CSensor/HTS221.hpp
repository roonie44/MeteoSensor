#ifndef HTS221_HPP_
#define HTS221_HPP_

class CHTS221
{
   void Init   (void);

private:
   static const unsigned char REG_ADDR_HTS221_WHO_AM_I      = 0x0F;
   static const unsigned char REG_VAL_HTS221_WHO_AM_I       = 0xBC;
   static const unsigned char REG_ADDR_HTS221_CTRL1         = 0x20;
   static const unsigned char REG_ADDR_HTS221_CTRL2         = 0x21;
   static const unsigned char REG_ADDR_HTS221_STATUS        = 0x27;
   static const unsigned char REG_ADDR_HTS221_HUMI_OUT      = (0x28 | 0x80);  // Autoincrement
   static const unsigned char REG_ADDR_HTS221_TEMP_OUT      = (0x2A | 0x80);  // Autoincrement
   static const unsigned char REG_ADDR_HTS221_CALIB_DATA    = (0x30 | 0x80);  // Autoincrement

#pragma pack(1)
   struct THTS221CalibData
   {
      unsigned char  u8H0_rH_x2;
      unsigned char  u8H1_rH_x2;
      unsigned char  u8T0_degC_x8;
      unsigned char  u8T1_degC_x8;
      unsigned char  Reserved1;
      unsigned char  b2T0_msb       : 2;
      unsigned char  b2T1_msb       : 2;
      unsigned char  Reserved2      : 4;
      signed short   s16H0_T0_OUT;
      signed short   Reserved3;
      signed short   s16H1_T0_OUT;
      signed short   s16T0_OUT;
      signed short   s16T1_OUT;
   };
#pragma pack()

   signed short   s16CalibrationT0ADC;
   signed short   s16CalibrationT1ADC;
   signed short   s16CalibrationT0;
   signed short   s16CalibrationT1;

   signed short   s16CalibrationH0ADC;
   signed short   s16CalibrationH1ADC;
   signed short   s16CalibrationH0;
   signed short   s16CalibrationH1;

   void           PowerUp                (void);
   void           PowerDown              (void);
   void           CheckState             (void);
   void           ReadCalibrationData    (void);
   void           NewMeasurement         (void);
   void           ReadTemperature        (void);
   void           ReadHumidity           (void);
};

#endif /* HTS221_HPP_ */
