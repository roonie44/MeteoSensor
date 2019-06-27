#include "main.h"

void CSensorModule::Init(void)
{
//   HTS221_PowerUp();
//   HTS221_CheckState();
//   HTS221_ReadCalibrationData();
//   HTS221_PowerDown();

//   LPS22HB_CheckState();

   BME280_ReadCalibrationData();
   //BME280_NewMeasurement();
   //BME280_ReadTemperature();
   //BME280_ReadHumidity();
   //BME280_ReadPressure();
}

void CSensorModule::Handle(void)
{
   HandleRequest = false;

   if(RequestDataType == TEMPERATURE)
   {
      //HTS221_PowerUp();
      //HTS221_NewMeasurement();
      //HTS221_ReadTemperature();
      //HTS221_PowerDown();
      BME280_NewMeasurement();
      BME280_ReadTemperature();
   }

   if(RequestDataType == HUMIDITY)
   {
      //HTS221_PowerUp();
      //HTS221_NewMeasurement();
      //HTS221_ReadHumidity();
      //HTS221_PowerDown();
      BME280_NewMeasurement();
      BME280_ReadTemperature();
      BME280_ReadHumidity();
   }

   if(RequestDataType == PRESSURE)
   {
      //LPS22HB_NewMeasurement();
      //LPS22HB_ReadPressure();
      BME280_NewMeasurement();
      BME280_ReadTemperature();
      BME280_ReadPressure();
   }
}

void CSensorModule::DataRequest(eDataType DataType, unsigned int u32CallbackId)
{
   this->RequestDataType   = DataType;
   this->HandleRequest     = true;
   this->u32CallbackId     = u32CallbackId;
}

void CSensorModule::HTS221_PowerUp(void)
{
   unsigned char U8RawData[2];

   Log.Str("SEN HTS221 Power UP\r");

   U8RawData[0] = REG_ADDR_HTS221_CTRL1;
   U8RawData[1] = 0x80;
   I2C.Write(I2C_ADDRESS_HTS221, U8RawData, sizeof(U8RawData));
}

void CSensorModule::HTS221_PowerDown(void)
{
   unsigned char U8RawData[2];

   return;

   Log.Str("SEN HTS221 Power DOWN\r");

   U8RawData[0] = REG_ADDR_HTS221_CTRL1;
   U8RawData[1] = 0x00;
   I2C.Write(I2C_ADDRESS_HTS221, U8RawData, sizeof(U8RawData));
}

void CSensorModule::HTS221_CheckState(void)
{
   unsigned char U8RawData[2];

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_WHO_AM_I);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], 1);

   Log.Str("SEN HTS221 Status: ");
   if(U8RawData[0] == REG_VAL_HTS221_WHO_AM_I)
      Log.Str("OK\r");
   else
      Log.Str("ERR\r");
}

void CSensorModule::HTS221_ReadCalibrationData(void)
{
   THTS221CalibData  HTS221CalibData;

   Log.Str("SEN HTS221 Calibration data read\r");

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_CALIB_DATA);
   I2C.Read(I2C_ADDRESS_HTS221, &HTS221CalibData, sizeof(HTS221CalibData));

   this->s16CalibrationT0ADC  = HTS221CalibData.s16T0_OUT;
   this->s16CalibrationT1ADC  = HTS221CalibData.s16T1_OUT;
   this->s16CalibrationT0     = HTS221CalibData.b2T0_msb;
   this->s16CalibrationT0   <<= 8;
   this->s16CalibrationT0    |= HTS221CalibData.u8T0_degC_x8;
   this->s16CalibrationT1     = HTS221CalibData.b2T1_msb;
   this->s16CalibrationT1   <<= 8;
   this->s16CalibrationT1    |= HTS221CalibData.u8T1_degC_x8;

   this->s16CalibrationH0ADC  = HTS221CalibData.s16H0_T0_OUT;
   this->s16CalibrationH1ADC  = HTS221CalibData.s16H1_T0_OUT;
   this->s16CalibrationH0     = HTS221CalibData.u8H0_rH_x2;
   this->s16CalibrationH1     = HTS221CalibData.u8H1_rH_x2;
}

void CSensorModule::HTS221_NewMeasurement(void)
{
   unsigned char U8RawData[2];
   unsigned int  u32Timeout = 100;

   Log.Str("SEN HTS221 Measurement request\r");

   // Flush pending readouts
   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_TEMP_OUT);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], sizeof(U8RawData));

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_HUMI_OUT);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], sizeof(U8RawData));

   // One-shot request
   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_CTRL2);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], 1);

   if(U8RawData[0] & 0x01)
   {
      U8RawData[0] = REG_ADDR_HTS221_CTRL2;
      U8RawData[1] = 0x00;
      I2C.Write(I2C_ADDRESS_HTS221, U8RawData, 2);

      I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_CTRL2);
      I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], 1);
   }

   U8RawData[0] = REG_ADDR_HTS221_CTRL2;
   U8RawData[1] = 0x01;
   I2C.Write(I2C_ADDRESS_HTS221, U8RawData, 2);

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_CTRL2);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], 1);

   do
   {
      // Read status
      I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_STATUS);
      I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], 1);
   }
   while((U8RawData[0] & 0x03) != 0x03 && u32Timeout-- != 0);
}

void CSensorModule::HTS221_ReadTemperature(void)
{
   unsigned char U8RawData[2];
   signed int s32Temperature;

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_TEMP_OUT);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], sizeof(U8RawData));

   s32Temperature    = *(signed short*)U8RawData;
   s32Temperature   -= this->s16CalibrationT0ADC;
   s32Temperature   *= this->s16CalibrationT1 - this->s16CalibrationT0;
   s32Temperature   /= this->s16CalibrationT1ADC - this->s16CalibrationT0ADC;
   s32Temperature   += this->s16CalibrationT0;

   Log.StrDec("SEN Temp ", s32Temperature / 8);
   Log.StrDecR(".", (s32Temperature % 8) * 125);

   s32Temperature *= 100;
   s32Temperature /= 8;

   if(u32CallbackId != 0)
      BlueNRG.Callback(u32CallbackId, &s32Temperature);
}

void CSensorModule::HTS221_ReadHumidity(void)
{
   unsigned char U8RawData[2];
   signed int s32Humidity;

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_HUMI_OUT);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], sizeof(U8RawData));

   s32Humidity       = *(signed short*)U8RawData;
   s32Humidity      -= this->s16CalibrationH0ADC;
   s32Humidity      *= this->s16CalibrationH1 - this->s16CalibrationH0;
   s32Humidity      /= this->s16CalibrationH1ADC - this->s16CalibrationH0ADC;
   s32Humidity      += this->s16CalibrationH0;

   Log.StrDec("SEN Humi ", s32Humidity / 2);
   Log.Str(" %\r");

   s32Humidity *= 100;
   s32Humidity /= 2;

   if(u32CallbackId != 0)
      BlueNRG.Callback(u32CallbackId, &s32Humidity);
}

void CSensorModule::LPS22HB_CheckState(void)
{
   unsigned char     U8RawData[2];

   Log.Str("SEN LPS22HB Power UP\r");

   I2C.Write(I2C_ADDRESS_LPS22HB, REG_ADDR_LPS22HB_WHO_AM_I);
   I2C.Read(I2C_ADDRESS_LPS22HB, &U8RawData[0], 1);

   Log.Str("SEN LPS22HB Status: ");
   if(U8RawData[0] == REG_VAL_LPS22HB_WHO_AM_I)
      Log.Str("OK\r");
   else
      Log.Str("ERR\r");
}

void CSensorModule::LPS22HB_NewMeasurement(void)
{
   unsigned char U8RawData[3];

   Log.Str("SEN LPS22HB Measurement request\r");

   // Flush pending readouts
   I2C.Write(I2C_ADDRESS_LPS22HB, REG_ADDR_LPS22HB_PRESS_OUT_XL);
   I2C.Read(I2C_ADDRESS_LPS22HB, &U8RawData[0], sizeof(U8RawData));

   // One-shot request with auto increment of register address
   U8RawData[0] = REG_ADDR_LPS22HB_CTRL2;
   U8RawData[1] = 0x11;
   I2C.Write(I2C_ADDRESS_LPS22HB, U8RawData, 2);

   do
   {
      // Read status
      I2C.Write(I2C_ADDRESS_LPS22HB, REG_ADDR_LPS22HB_STATUS);
      I2C.Read(I2C_ADDRESS_LPS22HB, &U8RawData[0], 1);
   }
   while((U8RawData[0] & 0x03) != 0x03);
}

void CSensorModule::LPS22HB_ReadPressure(void)
{
   unsigned char  U8RawData[3];
   unsigned int   u32Pressure = 0;

   I2C.Write(I2C_ADDRESS_LPS22HB, REG_ADDR_LPS22HB_PRESS_OUT_XL);
   I2C.Read(I2C_ADDRESS_LPS22HB, &U8RawData[0], sizeof(U8RawData));

   // Pressure in 1/4096 [hPa]
   u32Pressure = (unsigned int)U8RawData[0] | (unsigned int)U8RawData[1] << 8 | (unsigned int)U8RawData[2] << 16;

   Log.StrDecR("SEN Pressure ", u32Pressure / 4096);

   // Pressure in 0.1 [Pa] = 1/4096 [hPa] * 1000 / 4096
   u32Pressure = (u32Pressure * 250) / 1024;

   if(u32CallbackId != 0)
      BlueNRG.Callback(u32CallbackId, &u32Pressure);
}

void CSensorModule::BME280_ReadCalibrationData(void)
{
   Log.Str("SEN BME280 Calibration data read\r");

   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_T1);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.u16T1, sizeof(BME280_CalibrationData.u16T1));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_T2);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16T2, sizeof(BME280_CalibrationData.s16T2));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_T3);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16T3, sizeof(BME280_CalibrationData.s16T3));

   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_P1);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.u16P1, sizeof(BME280_CalibrationData.u16P1));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_P2);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16P2, sizeof(BME280_CalibrationData.s16P2));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_P3);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16P3, sizeof(BME280_CalibrationData.s16P3));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_P4);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16P4, sizeof(BME280_CalibrationData.s16P4));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_P5);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16P5, sizeof(BME280_CalibrationData.s16P5));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_P6);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16P6, sizeof(BME280_CalibrationData.s16P6));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_P7);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16P7, sizeof(BME280_CalibrationData.s16P7));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_P8);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16P8, sizeof(BME280_CalibrationData.s16P8));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_P9);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16P9, sizeof(BME280_CalibrationData.s16P9));

   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_H1);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.u8H1, sizeof(BME280_CalibrationData.u8H1));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_H2);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16H2, sizeof(BME280_CalibrationData.s16H2));
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_H3);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.u8H3, sizeof(BME280_CalibrationData.u8H3));

   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_H4);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16H4, sizeof(BME280_CalibrationData.s16H4));
   BME280_CalibrationData.s16H4 = (BME280_CalibrationData.s16H4 << 8) | (BME280_CalibrationData.s16H4 >> 8);
   BME280_CalibrationData.s16H4 = BME280_CalibrationData.s16H4 >> 4;
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_H5);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s16H5, sizeof(BME280_CalibrationData.s16H5));
   BME280_CalibrationData.s16H5 = BME280_CalibrationData.s16H5 >> 4;
   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_CALIB_H6);
   I2C.Read(I2C_ADDRESS_BME280, &BME280_CalibrationData.s8H6, sizeof(BME280_CalibrationData.s8H6));
}



void CSensorModule::BME280_NewMeasurement(void)
{
   unsigned char U8RawData[3];

   U8RawData[0] = REG_ADDR_BME280_CTRL_HUM;
   U8RawData[1] = 0x01;
   I2C.Write(I2C_ADDRESS_BME280, U8RawData, 2);

   U8RawData[0] = REG_ADDR_BME280_CTRL_MEAS;
   U8RawData[1] = 0x25;
   I2C.Write(I2C_ADDRESS_BME280, U8RawData, 2);

   do
   {
      // Read status
      I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_STATUS);
      I2C.Read(I2C_ADDRESS_BME280, &U8RawData[0], 1);
   }
   while((U8RawData[0] & 0x08) != 0x00);
}
void CSensorModule::BME280_ReadTemperature(void)
{
   unsigned char U8RawData[3];
   signed int s32TemperatureRaw, s32Temperature;

   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_TEMP);
   I2C.Read(I2C_ADDRESS_BME280, U8RawData, sizeof(U8RawData));

   s32TemperatureRaw    = U8RawData[0];
   s32TemperatureRaw    = s32TemperatureRaw << 8;
   s32TemperatureRaw   |= U8RawData[1];
   s32TemperatureRaw    = s32TemperatureRaw << 4;
   s32TemperatureRaw   |= U8RawData[2] & 0x0F;
   Log.StrDecR("SEN TempRaw ", s32TemperatureRaw);

   s32Temperature = BME280_CompensateTemperature(s32TemperatureRaw);

   Log.StrDec("SEN Temp ", s32Temperature / 100);
   Log.StrDecR(".", s32Temperature % 100);

   if(u32CallbackId != 0)
      BlueNRG.Callback(u32CallbackId, &s32Temperature);
}

void CSensorModule::BME280_ReadHumidity(void)
{
   unsigned char U8RawData[2];
   signed int s32HumidityRaw;
   unsigned int u32Humidity;

   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_HUM);
   I2C.Read(I2C_ADDRESS_BME280, &U8RawData[0], sizeof(U8RawData));

   s32HumidityRaw    = U8RawData[0];
   s32HumidityRaw    = s32HumidityRaw << 8;
   s32HumidityRaw   |= U8RawData[1];
   Log.StrDecR("SEN HumiRaw ", s32HumidityRaw);

   u32Humidity = BME280_CompensateHumidity(s32HumidityRaw);
   Log.StrDecR("SEN Humidity ", u32Humidity);

   if(u32CallbackId != 0)
      BlueNRG.Callback(u32CallbackId, &u32Humidity);
}

void CSensorModule::BME280_ReadPressure(void)
{
   unsigned char  U8RawData[3];
   unsigned int   u32PressureRaw, u32Pressure;

   I2C.Write(I2C_ADDRESS_BME280, REG_ADDR_BME280_PRESS);
   I2C.Read(I2C_ADDRESS_BME280, U8RawData, sizeof(U8RawData));

   u32PressureRaw    = U8RawData[0];
   u32PressureRaw    = u32PressureRaw << 8;
   u32PressureRaw   |= U8RawData[1];
   u32PressureRaw    = u32PressureRaw << 4;
   u32PressureRaw   |= U8RawData[2] & 0x0F;
   Log.StrDecR("SEN PressRaw ", u32PressureRaw);

   u32Pressure = BME280_CompensatePressure(u32PressureRaw) / 256;

   Log.StrDecR("SEN Pressure ", u32Pressure);

   if(u32CallbackId != 0)
      BlueNRG.Callback(u32CallbackId, &u32Pressure);
}

signed int CSensorModule::BME280_CompensateTemperature(signed int s32TemperatureRaw)
{
   signed int s32Var1, s32Var2, s32Temperature;

   s32Var1 = ((((s32TemperatureRaw >> 3) - ((signed int)BME280_CalibrationData.u16T1 << 1))) * ((signed int)BME280_CalibrationData.s16T2)) >> 11;
   s32Var2 = (((((s32TemperatureRaw >> 4) - ((signed int)BME280_CalibrationData.u16T1)) * ((s32TemperatureRaw >> 4) - ((signed int)BME280_CalibrationData.u16T1))) >> 12) * ((signed int)BME280_CalibrationData.s16T3)) >> 14;
   BME280_TemperatureFine = s32Var1 + s32Var2;
   s32Temperature = (BME280_TemperatureFine * 5 + 128) >> 8;
   return s32Temperature;
}

unsigned int CSensorModule::BME280_CompensateHumidity(signed int s32HumidityRaw)
{
   signed int s32Var1;

   s32Var1 = BME280_TemperatureFine - (signed int)76800;
   s32Var1 = (((((s32HumidityRaw << 14) - (((signed int)BME280_CalibrationData.s16H4) << 20) - (((signed int)BME280_CalibrationData.s16H5) * s32Var1)) + ((signed int)16384)) >> 15) * (((((((s32Var1 * ((signed int)BME280_CalibrationData.s8H6)) >> 10) * (((s32Var1 * ((signed int)BME280_CalibrationData.u8H3)) >> 11) + ((signed int)32768))) >> 10) + ((signed int)2097152)) * ((signed int)BME280_CalibrationData.s16H2) + 8192) >> 14));
   s32Var1 = (s32Var1 - (((((s32Var1 >> 15) * (s32Var1 >> 15)) >> 7) * ((signed int)BME280_CalibrationData.u8H1)) >> 4));
   s32Var1 = (s32Var1 < 0 ? 0 : s32Var1);
   s32Var1 = (s32Var1 > 419430400 ? 419430400 : s32Var1);
   return (unsigned int)(s32Var1 >> 12);
}

unsigned int CSensorModule::BME280_CompensatePressure(signed int s32PressureRaw)
{
   signed long long  s64Var1, s64Var2, s64Pressure;

   s64Var1 = (signed long long)BME280_TemperatureFine - 128000;
   s64Var2 = s64Var1 * s64Var1 * (signed long long)BME280_CalibrationData.s16P6;
   s64Var2 = s64Var2 + ((s64Var1 * (signed long long)BME280_CalibrationData.s16P5) << 17);
   s64Var2 = s64Var2 + (((signed long long)BME280_CalibrationData.s16P4) << 35);
   s64Var1 = ((s64Var1 * s64Var1 * (signed long long)BME280_CalibrationData.s16P3) >> 8) + ((s64Var1 * (signed long long)BME280_CalibrationData.s16P2) << 12);
   s64Var1 = (((((signed long long)1) << 47) + s64Var1)) * ((signed long long)BME280_CalibrationData.u16P1) >> 33;
   if (s64Var1 == 0)
      return 0;
   s64Pressure = 1048576 - s32PressureRaw;
   s64Pressure = (((s64Pressure << 31) - s64Var2) * 3125) / s64Var1;
   s64Var1 = (((signed long long)BME280_CalibrationData.s16P9) * (s64Pressure >> 13) * (s64Pressure >> 13)) >> 25;
   s64Var2 = (((signed long long)BME280_CalibrationData.s16P8) * s64Pressure) >> 19;
   s64Pressure = ((s64Pressure + s64Var1 + s64Var2) >> 8) + (((signed long long)BME280_CalibrationData.s16P7) << 4);
   return (unsigned int)s64Pressure;
}
