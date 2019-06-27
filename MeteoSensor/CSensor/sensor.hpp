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

   static const unsigned char REG_ADDR_BME280_CALIB_T1      = 0x88;
   static const unsigned char REG_ADDR_BME280_CALIB_T2      = 0x8A;
   static const unsigned char REG_ADDR_BME280_CALIB_T3      = 0x8C;
   static const unsigned char REG_ADDR_BME280_CALIB_P1      = 0x8E;
   static const unsigned char REG_ADDR_BME280_CALIB_P2      = 0x90;
   static const unsigned char REG_ADDR_BME280_CALIB_P3      = 0x92;
   static const unsigned char REG_ADDR_BME280_CALIB_P4      = 0x94;
   static const unsigned char REG_ADDR_BME280_CALIB_P5      = 0x96;
   static const unsigned char REG_ADDR_BME280_CALIB_P6      = 0x98;
   static const unsigned char REG_ADDR_BME280_CALIB_P7      = 0x9A;
   static const unsigned char REG_ADDR_BME280_CALIB_P8      = 0x9C;
   static const unsigned char REG_ADDR_BME280_CALIB_P9      = 0x9E;
   static const unsigned char REG_ADDR_BME280_CALIB_H1      = 0xA1;
   static const unsigned char REG_ADDR_BME280_CALIB_H2      = 0xE1;
   static const unsigned char REG_ADDR_BME280_CALIB_H3      = 0xE3;
   static const unsigned char REG_ADDR_BME280_CALIB_H4      = 0xE4;
   static const unsigned char REG_ADDR_BME280_CALIB_H5      = 0xE5;
   static const unsigned char REG_ADDR_BME280_CALIB_H6      = 0xE7;
   static const unsigned char REG_ADDR_BME280_CTRL_HUM      = 0xF2;
   static const unsigned char REG_ADDR_BME280_STATUS        = 0xF3;
   static const unsigned char REG_ADDR_BME280_CTRL_MEAS     = 0xF4;
   static const unsigned char REG_ADDR_BME280_CONFIG        = 0xF5;
   static const unsigned char REG_ADDR_BME280_PRESS         = 0xF7;
   static const unsigned char REG_ADDR_BME280_TEMP          = 0xFA;
   static const unsigned char REG_ADDR_BME280_HUM           = 0xFD;

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

   struct
   {
      unsigned short u16T1;
        signed short s16T2;
        signed short s16T3;
      unsigned short u16P1;
        signed short s16P2;
        signed short s16P3;
        signed short s16P4;
        signed short s16P5;
        signed short s16P6;
        signed short s16P7;
        signed short s16P8;
        signed short s16P9;
      unsigned char  u8H1;
        signed short s16H2;
      unsigned char  u8H3;
        signed short s16H4;
        signed short s16H5;
        signed char  s8H6;
   }BME280_CalibrationData;

   signed int BME280_TemperatureFine;

   void           HTS221_PowerUp                (void);
   void           HTS221_PowerDown              (void);
   void           HTS221_CheckState             (void);
   void           HTS221_ReadCalibrationData    (void);
   void           HTS221_NewMeasurement         (void);
   void           HTS221_ReadTemperature        (void);
   void           HTS221_ReadHumidity           (void);

   void           LPS22HB_CheckState            (void);
   void           LPS22HB_NewMeasurement        (void);
   void           LPS22HB_ReadPressure          (void);

   void           BME280_ReadCalibrationData    (void);
   void           BME280_NewMeasurement         (void);
   void           BME280_ReadTemperature        (void);
   void           BME280_ReadHumidity           (void);
   void           BME280_ReadPressure           (void);
   signed int     BME280_CompensateTemperature  (signed int s32TemperatureRaw);
   unsigned int   BME280_CompensateHumidity     (signed int s32HumidityRaw);
   unsigned int   BME280_CompensatePressure     (signed int s32PressureRaw);
};

#endif /* SENSOR_HPP_ */
