#pragma once

class CBME280
{
public:
                  CBME280                 (CI2C *pI2C) { this->pI2C = pI2C; }

   void           Init                    ();
   Status         GetTemperature          (signed int& s32Temperature);
   Status         GetHumidity             (unsigned int& u32Humidity);
   Status         GetPressure             (unsigned int& u32Pressure);

private:
   static const unsigned int  POWER_ON_TIMEOUT           = 10; // ms

   static const unsigned char I2C_ADDR                   = 0x76;

   static const unsigned char REG_ADDR_CALIB_T1          = 0x88;
   static const unsigned char REG_ADDR_CALIB_T2          = 0x8A;
   static const unsigned char REG_ADDR_CALIB_T3          = 0x8C;
   static const unsigned char REG_ADDR_CALIB_P1          = 0x8E;
   static const unsigned char REG_ADDR_CALIB_P2          = 0x90;
   static const unsigned char REG_ADDR_CALIB_P3          = 0x92;
   static const unsigned char REG_ADDR_CALIB_P4          = 0x94;
   static const unsigned char REG_ADDR_CALIB_P5          = 0x96;
   static const unsigned char REG_ADDR_CALIB_P6          = 0x98;
   static const unsigned char REG_ADDR_CALIB_P7          = 0x9A;
   static const unsigned char REG_ADDR_CALIB_P8          = 0x9C;
   static const unsigned char REG_ADDR_CALIB_P9          = 0x9E;
   static const unsigned char REG_ADDR_CALIB_H1          = 0xA1;
   static const unsigned char REG_ADDR_ID                = 0xD0;
   static const unsigned char REG_ADDR_CALIB_H2          = 0xE1;
   static const unsigned char REG_ADDR_CALIB_H3          = 0xE3;
   static const unsigned char REG_ADDR_CALIB_H4          = 0xE4;
   static const unsigned char REG_ADDR_CALIB_H5          = 0xE5;
   static const unsigned char REG_ADDR_CALIB_H6          = 0xE7;
   static const unsigned char REG_ADDR_CTRL_HUM          = 0xF2;
   static const unsigned char REG_ADDR_STATUS            = 0xF3;
   static const unsigned char REG_ADDR_CTRL_MEAS         = 0xF4;
   static const unsigned char REG_ADDR_CONFIG            = 0xF5;
   static const unsigned char REG_ADDR_PRESS             = 0xF7;
   static const unsigned char REG_ADDR_TEMP              = 0xFA;
   static const unsigned char REG_ADDR_HUM               = 0xFD;

   static const unsigned char REG_VAL_CHIP_ID            = 0x60;

   static const unsigned char REG_MASK_CTRL_MEAS_MODE    = 0x01;  // forced
   static const unsigned char REG_MASK_CTRL_MEAS_OSRS_P  = 0x04;  // oversampling x1
   static const unsigned char REG_MASK_CTRL_MEAS_OSRS_T  = 0x20;  // oversampling x1
   static const unsigned char REG_MASK_CTRL_HUM_OSRS_H   = 0x01;  // oversampling x1
   static const unsigned char REG_MASK_STATUS_MEASURING  = 0x08;  // 1 = conversion running

   CI2C              *pI2C;

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
   }CalibrationData;

   signed int TemperatureFine;

   void           ReadCalibrationData     ();
   unsigned char  ReadChipId              ();
   void           NewMeasurement          (bool MeasureTemperature, bool MeasureHumidity, bool MeasurePressure);
   signed int     ReadTemperature         ();
   unsigned int   ReadHumidity            ();
   unsigned int   ReadPressure            ();
   Status         PowerOn                 ();
   void           PowerOff                ();
   signed int     CompensateTemperature   (signed int s32TemperatureRaw);
   unsigned int   CompensateHumidity      (signed int s32HumidityRaw);
   unsigned int   CompensatePressure      (signed int s32PressureRaw);
};
