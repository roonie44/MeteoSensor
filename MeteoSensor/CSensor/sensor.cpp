#include "main.h"

void CSensorModule::Init(void)
{
   THTS221CalibData  HTS221CalibData;

   HTS221_PowerUp();

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

void CSensorModule::Handle(void)
{
   HandleRequest = false;

   if(RequestDataType != NONE)
   {
      //HTS221_PowerUp();
      HTS221_NewMeasurement();
   }

   if(RequestDataType == TEMPERATURE)
      HTS221_ReadTemperature();

   if(RequestDataType == HUMIDITY)
      HTS221_ReadHumidity();

   //HTS221_PowerDown();
}

void CSensorModule::DataRequest(eDataType DataType, unsigned int u32CallbackId)
{
   this->RequestDataType   = DataType;
   this->HandleRequest     = true;
   this->u32CallbackId     = u32CallbackId;
}

void CSensorModule::HTS221_PowerUp(void)
{
   unsigned char     U8RawData[2];

   U8RawData[0] = REG_ADDR_HTS221_CTRL1;
   U8RawData[1] = 0x80;
   I2C.Write(I2C_ADDRESS_HTS221, U8RawData, sizeof(U8RawData));
}

void CSensorModule::HTS221_PowerDown(void)
{
   unsigned char     U8RawData[2];

   U8RawData[0] = REG_ADDR_HTS221_CTRL1;
   U8RawData[1] = 0x00;
   I2C.Write(I2C_ADDRESS_HTS221, U8RawData, sizeof(U8RawData));
}

void CSensorModule::HTS221_NewMeasurement(void)
{
   unsigned char U8RawData[2];

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_STATUS);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], 1);

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_TEMP_OUT);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], sizeof(U8RawData));

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_HUMI_OUT);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], sizeof(U8RawData));

   I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_STATUS);
   I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], 1);

   U8RawData[0] = REG_ADDR_HTS221_CTRL2;
   U8RawData[1] = 0x01;
   I2C.Write(I2C_ADDRESS_HTS221, U8RawData, 2);

   do
   {
      I2C.Write(I2C_ADDRESS_HTS221, REG_ADDR_HTS221_STATUS);
      I2C.Read(I2C_ADDRESS_HTS221, &U8RawData[0], 1);
   }
   while((U8RawData[0] & 0x03) != 0x03);
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

   BlueNRG.Callback(u32CallbackId, &s32Humidity);
}
