#include "main.h"

void CBME280::Init()
{
   if (PowerOn() == Status::OK)
   {
      ReadCalibrationData();
      PowerOff();
   }
}
// Returns 0.01 DegC
Status CBME280::GetTemperature(signed int& s32Temperature)
{
   Status PowerOnStatus = PowerOn();
   if (PowerOnStatus != Status::OK)
      return PowerOnStatus;

   NewMeasurement(true, false, false);
   s32Temperature = ReadTemperature();
   PowerOff();

   Log.StrDec("BME Temp ", s32Temperature / 100);
   Log.StrDecR(".", s32Temperature % 100);
   return Status::OK;
}
// Returns 0.01 %
Status CBME280::GetHumidity(unsigned int& u32Humidity)
{
   Status PowerOnStatus = PowerOn();
   if (PowerOnStatus != Status::OK)
      return PowerOnStatus;

   NewMeasurement(true, true, false);
   u32Humidity = ReadHumidity() * 100 / 1024;
   PowerOff();

   Log.StrDec("BME Humidity ", u32Humidity / 100);
   Log.StrDecR(".", u32Humidity % 100);
   return Status::OK;
}
// Returns
Status CBME280::GetPressure(unsigned int& u32Pressure)
{
   Status PowerOnStatus = PowerOn();
   if (PowerOnStatus != Status::OK)
      return PowerOnStatus;

   NewMeasurement(true, false, true);
   u32Pressure = ReadPressure() * 10 / 256;
   PowerOff();

   Log.StrDec("BME Pressure ", u32Pressure / 1000);
   Log.StrDecR(".", u32Pressure % 1000);
   return Status::OK;
}

Status CBME280::PowerOn()
{
   CClock   Clock;
   Log.Str("BME Power ON ");
   LL_GPIO_ResetOutputPin(PIN_SENSOR_I2C2_PWR_PORT, PIN_SENSOR_I2C2_PWR_PIN);
   LL_GPIO_SetPinMode(PIN_SENSOR_I2C2_SCL_PORT, PIN_SENSOR_I2C2_SCL_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinMode(PIN_SENSOR_I2C2_SDA_PORT, PIN_SENSOR_I2C2_SDA_PIN, LL_GPIO_MODE_ALTERNATE);

   while (LL_GPIO_IsInputPinSet(PIN_SENSOR_I2C2_SCL_PORT, PIN_SENSOR_I2C2_SCL_PIN) == 0 ||
          LL_GPIO_IsInputPinSet(PIN_SENSOR_I2C2_SDA_PORT, PIN_SENSOR_I2C2_SDA_PIN) == 0)
   {
      if (Clock.IsElapsed(POWER_ON_TIMEOUT))
      {
         Log.Str(" err\r");
         PowerOff();
         return Status::TIMEOUT;
      }
   }
   Log.Str("\r");
   LL_I2C_Enable(I2C2);

   while (ReadChipId() != REG_VAL_CHIP_ID)
   {
      if (Clock.IsElapsed(POWER_ON_TIMEOUT))
      {
         PowerOff();
         return Status::FAILURE;
      }
   }

   return Status::OK;
}
void CBME280::PowerOff()
{
   Log.Str("BME Power OFF\r");
   LL_I2C_Disable(I2C2);
   LL_GPIO_SetPinMode(PIN_SENSOR_I2C2_SDA_PORT, PIN_SENSOR_I2C2_SDA_PIN, LL_GPIO_MODE_ANALOG);
   LL_GPIO_SetPinMode(PIN_SENSOR_I2C2_SCL_PORT, PIN_SENSOR_I2C2_SCL_PIN, LL_GPIO_MODE_ANALOG);
   LL_GPIO_SetOutputPin(PIN_SENSOR_I2C2_PWR_PORT, PIN_SENSOR_I2C2_PWR_PIN);
}

void CBME280::ReadCalibrationData()
{
   Log.Str("BME Calibration data read\r");

   pI2C->SetSlaveAddress(I2C_ADDRESS_BME280);
   pI2C->Write(REG_ADDR_CALIB_T1);
   pI2C->Read(&CalibrationData.u16T1, sizeof(CalibrationData.u16T1));
   pI2C->Write(REG_ADDR_CALIB_T2);
   pI2C->Read(&CalibrationData.s16T2, sizeof(CalibrationData.s16T2));
   pI2C->Write(REG_ADDR_CALIB_T3);
   pI2C->Read(&CalibrationData.s16T3, sizeof(CalibrationData.s16T3));

   pI2C->Write(REG_ADDR_CALIB_P1);
   pI2C->Read(&CalibrationData.u16P1, sizeof(CalibrationData.u16P1));
   pI2C->Write(REG_ADDR_CALIB_P2);
   pI2C->Read(&CalibrationData.s16P2, sizeof(CalibrationData.s16P2));
   pI2C->Write(REG_ADDR_CALIB_P3);
   pI2C->Read(&CalibrationData.s16P3, sizeof(CalibrationData.s16P3));
   pI2C->Write(REG_ADDR_CALIB_P4);
   pI2C->Read(&CalibrationData.s16P4, sizeof(CalibrationData.s16P4));
   pI2C->Write(REG_ADDR_CALIB_P5);
   pI2C->Read(&CalibrationData.s16P5, sizeof(CalibrationData.s16P5));
   pI2C->Write(REG_ADDR_CALIB_P6);
   pI2C->Read(&CalibrationData.s16P6, sizeof(CalibrationData.s16P6));
   pI2C->Write(REG_ADDR_CALIB_P7);
   pI2C->Read(&CalibrationData.s16P7, sizeof(CalibrationData.s16P7));
   pI2C->Write(REG_ADDR_CALIB_P8);
   pI2C->Read(&CalibrationData.s16P8, sizeof(CalibrationData.s16P8));
   pI2C->Write(REG_ADDR_CALIB_P9);
   pI2C->Read(&CalibrationData.s16P9, sizeof(CalibrationData.s16P9));

   pI2C->Write(REG_ADDR_CALIB_H1);
   pI2C->Read(&CalibrationData.u8H1, sizeof(CalibrationData.u8H1));
   pI2C->Write(REG_ADDR_CALIB_H2);
   pI2C->Read(&CalibrationData.s16H2, sizeof(CalibrationData.s16H2));
   pI2C->Write(REG_ADDR_CALIB_H3);
   pI2C->Read(&CalibrationData.u8H3, sizeof(CalibrationData.u8H3));

   pI2C->Write(REG_ADDR_CALIB_H4);
   pI2C->Read(&CalibrationData.s16H4, sizeof(CalibrationData.s16H4));
   CalibrationData.s16H4 = (CalibrationData.s16H4 << 8) | (CalibrationData.s16H4 >> 8);
   CalibrationData.s16H4 = CalibrationData.s16H4 >> 4;
   pI2C->Write(REG_ADDR_CALIB_H5);
   pI2C->Read(&CalibrationData.s16H5, sizeof(CalibrationData.s16H5));
   CalibrationData.s16H5 = CalibrationData.s16H5 >> 4;
   pI2C->Write(REG_ADDR_CALIB_H6);
   pI2C->Read(&CalibrationData.s8H6, sizeof(CalibrationData.s8H6));
}

unsigned char CBME280::ReadChipId()
{
   unsigned char u8ChipId;

   Log.Str("BME Read chip ID\r");

   pI2C->SetSlaveAddress(I2C_ADDRESS_BME280);
   pI2C->Write(REG_ADDR_ID);
   pI2C->Read(&u8ChipId, sizeof(u8ChipId));

   return u8ChipId;
}

void CBME280::NewMeasurement(bool MeasureTemperature, bool MeasureHumidity, bool MeasurePressure)
{
   unsigned char U8RawData[3];

   pI2C->SetSlaveAddress(I2C_ADDRESS_BME280);

   if (MeasureHumidity == true)
   {
      U8RawData[0] = REG_ADDR_CTRL_HUM;
      U8RawData[1] = REG_MASK_CTRL_HUM_OSRS_H;
      pI2C->Write(U8RawData, 2);
   }

   U8RawData[0] = REG_ADDR_CTRL_MEAS;
   U8RawData[1] = REG_MASK_CTRL_MEAS_MODE;
   if (MeasureTemperature == true)
      U8RawData[1] |= REG_MASK_CTRL_MEAS_OSRS_T;
   if (MeasurePressure == true)
      U8RawData[1] |= REG_MASK_CTRL_MEAS_OSRS_P;
   pI2C->Write(U8RawData, 2);

   do
   {
      pI2C->Write(REG_ADDR_STATUS);
      pI2C->Read(&U8RawData[0], 1);
   }
   while((U8RawData[0] & REG_MASK_STATUS_MEASURING) != 0);
}
// Returns temperature in DegC, resolution 0.01 DegC.
signed int CBME280::ReadTemperature()
{
   unsigned char U8RawData[3];
   signed int s32TemperatureRaw;

   pI2C->SetSlaveAddress(I2C_ADDRESS_BME280);
   pI2C->Write(REG_ADDR_TEMP);
   pI2C->Read(U8RawData, sizeof(U8RawData));

   s32TemperatureRaw    = U8RawData[0];
   s32TemperatureRaw    = s32TemperatureRaw << 8;
   s32TemperatureRaw   |= U8RawData[1];
   s32TemperatureRaw    = s32TemperatureRaw << 4;
   s32TemperatureRaw   |= U8RawData[2] & 0x0F;

   return CompensateTemperature(s32TemperatureRaw);
}
// Returns humidity in RH in Q22.10 format (22 integer bits and 10 fractional bits)
unsigned int CBME280::ReadHumidity()
{
   unsigned char U8RawData[2];
   signed int s32HumidityRaw;

   pI2C->SetSlaveAddress(I2C_ADDRESS_BME280);
   pI2C->Write(REG_ADDR_HUM);
   pI2C->Read(&U8RawData[0], sizeof(U8RawData));

   s32HumidityRaw    = U8RawData[0];
   s32HumidityRaw    = s32HumidityRaw << 8;
   s32HumidityRaw   |= U8RawData[1];

   return CompensateHumidity(s32HumidityRaw);
}
// Returns pressure in Pa in Q24.8 format (24 integer bits and 8 fractional bits)
unsigned int CBME280::ReadPressure()
{
   unsigned char  U8RawData[3];
   unsigned int   u32PressureRaw;

   pI2C->SetSlaveAddress(I2C_ADDRESS_BME280);
   pI2C->Write(REG_ADDR_PRESS);
   pI2C->Read(U8RawData, sizeof(U8RawData));

   u32PressureRaw    = U8RawData[0];
   u32PressureRaw    = u32PressureRaw << 8;
   u32PressureRaw   |= U8RawData[1];
   u32PressureRaw    = u32PressureRaw << 4;
   u32PressureRaw   |= U8RawData[2] & 0x0F;

   return CompensatePressure(u32PressureRaw);
}

signed int CBME280::CompensateTemperature(signed int s32TemperatureRaw)
{
   signed int s32Var1, s32Var2, s32Temperature;

   s32Var1 = ((((s32TemperatureRaw >> 3) - ((signed int)CalibrationData.u16T1 << 1))) * ((signed int)CalibrationData.s16T2)) >> 11;
   s32Var2 = (((((s32TemperatureRaw >> 4) - ((signed int)CalibrationData.u16T1)) * ((s32TemperatureRaw >> 4) - ((signed int)CalibrationData.u16T1))) >> 12) * ((signed int)CalibrationData.s16T3)) >> 14;
   TemperatureFine = s32Var1 + s32Var2;
   s32Temperature = (TemperatureFine * 5 + 128) >> 8;
   return s32Temperature;
}

unsigned int CBME280::CompensateHumidity(signed int s32HumidityRaw)
{
   signed int s32Var1;

   s32Var1 = TemperatureFine - (signed int)76800;
   s32Var1 = (((((s32HumidityRaw << 14) - (((signed int)CalibrationData.s16H4) << 20) - (((signed int)CalibrationData.s16H5) * s32Var1)) + ((signed int)16384)) >> 15) * (((((((s32Var1 * ((signed int)CalibrationData.s8H6)) >> 10) * (((s32Var1 * ((signed int)CalibrationData.u8H3)) >> 11) + ((signed int)32768))) >> 10) + ((signed int)2097152)) * ((signed int)CalibrationData.s16H2) + 8192) >> 14));
   s32Var1 = (s32Var1 - (((((s32Var1 >> 15) * (s32Var1 >> 15)) >> 7) * ((signed int)CalibrationData.u8H1)) >> 4));
   s32Var1 = (s32Var1 < 0 ? 0 : s32Var1);
   s32Var1 = (s32Var1 > 419430400 ? 419430400 : s32Var1);
   return (unsigned int)(s32Var1 >> 12);
}

unsigned int CBME280::CompensatePressure(signed int s32PressureRaw)
{
   signed long long  s64Var1, s64Var2, s64Pressure;

   s64Var1 = (signed long long)TemperatureFine - 128000;
   s64Var2 = s64Var1 * s64Var1 * (signed long long)CalibrationData.s16P6;
   s64Var2 = s64Var2 + ((s64Var1 * (signed long long)CalibrationData.s16P5) << 17);
   s64Var2 = s64Var2 + (((signed long long)CalibrationData.s16P4) << 35);
   s64Var1 = ((s64Var1 * s64Var1 * (signed long long)CalibrationData.s16P3) >> 8) + ((s64Var1 * (signed long long)CalibrationData.s16P2) << 12);
   s64Var1 = (((((signed long long)1) << 47) + s64Var1)) * ((signed long long)CalibrationData.u16P1) >> 33;
   if (s64Var1 == 0)
      return 0;
   s64Pressure = 1048576 - s32PressureRaw;
   s64Pressure = (((s64Pressure << 31) - s64Var2) * 3125) / s64Var1;
   s64Var1 = (((signed long long)CalibrationData.s16P9) * (s64Pressure >> 13) * (s64Pressure >> 13)) >> 25;
   s64Var2 = (((signed long long)CalibrationData.s16P8) * s64Pressure) >> 19;
   s64Pressure = ((s64Pressure + s64Var1 + s64Var2) >> 8) + (((signed long long)CalibrationData.s16P7) << 4);
   return (unsigned int)s64Pressure;
}
