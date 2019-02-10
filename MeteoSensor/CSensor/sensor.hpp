#ifndef SENSOR_HPP_
#define SENSOR_HPP_

class CSensorModule
{
public:
   enum eDataType
   {
      NONE,
      TEMPERATURE,
      HUMIDITY,
      PRESSURE,
   };

   bool  HandleRequest;

   void Init   (void);
   void Handle (void);
   void DataRequest  (eDataType DataType, unsigned int u32CallbackId);

private:
   static const unsigned char REG_ADDR_HTS221_WHO_AM_I      = 0x0F;
   static const unsigned char REG_VAL_HTS221_WHO_AM_I       = 0xBC;
   static const unsigned char REG_ADDR_HTS221_CTRL1         = 0x20;
   static const unsigned char REG_ADDR_HTS221_CTRL2         = 0x21;
   static const unsigned char REG_ADDR_HTS221_STATUS        = 0x27;
   static const unsigned char REG_ADDR_HTS221_HUMI_OUT      = (0x28 | 0x80);  // Autoincrement
   static const unsigned char REG_ADDR_HTS221_TEMP_OUT      = (0x2A | 0x80);  // Autoincrement
   static const unsigned char REG_ADDR_HTS221_CALIB_DATA    = (0x30 | 0x80);  // Autoincrement

   static const unsigned char REG_ADDR_LPS22HB_WHO_AM_I     = 0x0F;
   static const unsigned char REG_VAL_LPS22HB_WHO_AM_I      = 0xB1;
   static const unsigned char REG_ADDR_LPS22HB_CTRL1        = 0x10;
   static const unsigned char REG_ADDR_LPS22HB_CTRL2        = 0x11;
   static const unsigned char REG_ADDR_LPS22HB_STATUS       = 0x27;
   static const unsigned char REG_ADDR_LPS22HB_PRESS_OUT_XL = 0x28;


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

   eDataType      RequestDataType;
   unsigned int   u32CallbackId;

   void HTS221_PowerUp           (void);
   void HTS221_PowerDown         (void);
   void HTS221_NewMeasurement    (void);
   void HTS221_ReadTemperature   (void);
   void HTS221_ReadHumidity      (void);

   void LPS22HB_PowerUp          (void);
   void LPS22HB_PowerDown        (void);
   void LPS22HB_NewMeasurement   (void);
   void LPS22HB_ReadPressure     (void);
};

#endif /* SENSOR_HPP_ */
