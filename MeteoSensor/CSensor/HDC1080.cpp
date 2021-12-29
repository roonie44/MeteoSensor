#include "main.h"

void CHDC1080::Init()
{
   PowerOn();

   unsigned char U8RawData[2];
   U8RawData[0] = REG_ADDR_ID_MANUFACTURER;
   pI2C->Write(U8RawData, 1);
   pI2C->Read(U8RawData, 2);
   Log.StrBlobR("HDC Manufacturer ID ", U8RawData, sizeof(U8RawData));

   U8RawData[0] = REG_ADDR_ID_DEVICE;
   pI2C->Write(U8RawData, 1);
   pI2C->Read(U8RawData, 2);
   Log.StrBlobR("HDC Device ID ", U8RawData, sizeof(U8RawData));

   PowerOff();
}

Status CHDC1080::GetTemperature(signed int& s32Temperature)
{
   unsigned char  U8RawData[2];
   unsigned short u16TemperatureRaw;

   U8RawData[0] = REG_ADDR_CONFIG;
   U8RawData[1] = 0x00;
   pI2C->Write(U8RawData, 2);

   U8RawData[0] = REG_ADDR_TEMP;
   pI2C->Write(U8RawData, 1);

   CClock Clock;
   while (Clock.IsElapsed(TIMEOUT_CONVERTION) == false);

   pI2C->Read(U8RawData, sizeof(U8RawData));

   u16TemperatureRaw = (unsigned short)U8RawData[0] << 8 | U8RawData[1];
   s32Temperature    = 165 * 100 * u16TemperatureRaw / __UINT16_MAX__;
   s32Temperature   -= 40 * 100;

   Log.StrDec("HDC Temperature ", s32Temperature / 100);
   Log.StrDecR(".", s32Temperature % 100);
   return Status::OK;
}

Status CHDC1080::GetHumidity(unsigned int& u32Humidity)
{
   unsigned char  U8RawData[2];
   unsigned short u16HumidityRaw;

   U8RawData[0] = REG_ADDR_CONFIG;
   U8RawData[1] = 0x00;
   pI2C->Write(U8RawData, 2);

   U8RawData[0] = REG_ADDR_HUMI;
   pI2C->Write(U8RawData, 1);

   CClock Clock;
   while (Clock.IsElapsed(TIMEOUT_CONVERTION) == false);

   pI2C->Read(U8RawData, sizeof(U8RawData));

   u16HumidityRaw = (unsigned short)U8RawData[0] << 8 | U8RawData[1];
   u32Humidity    = 100 * 100 * u16HumidityRaw / __UINT16_MAX__;

   Log.StrDec("HDC Humidity ", u32Humidity / 100);
   Log.StrDecR(".", u32Humidity % 100);
   return Status::OK;
}

Status CHDC1080::PowerOn()
{
   Log.Str("HDC Power ON\r");

   LL_GPIO_ResetOutputPin(PIN_SENSOR_I2C_PWR_PORT, PIN_SENSOR_I2C_PWR_PIN);
   LL_GPIO_SetPinMode(PIN_SENSOR_I2C_SCL_PORT, PIN_SENSOR_I2C_SCL_PIN, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinMode(PIN_SENSOR_I2C_SDA_PORT, PIN_SENSOR_I2C_SDA_PIN, LL_GPIO_MODE_ALTERNATE);

   CClock Clock;
   while (Clock.IsElapsed(TIMEOUT_POWER_ON) == false);

   pI2C->Enable();
   pI2C->SetSlaveAddress(I2C_ADDR);

   return Status::OK;
}
void CHDC1080::PowerOff()
{
   Log.Str("HDC Power OFF\r");

   pI2C->Disable();

   LL_GPIO_SetPinMode(PIN_SENSOR_I2C_SDA_PORT, PIN_SENSOR_I2C_SDA_PIN, LL_GPIO_MODE_ANALOG);
   LL_GPIO_SetPinMode(PIN_SENSOR_I2C_SCL_PORT, PIN_SENSOR_I2C_SCL_PIN, LL_GPIO_MODE_ANALOG);
   LL_GPIO_SetOutputPin(PIN_SENSOR_I2C_PWR_PORT, PIN_SENSOR_I2C_PWR_PIN);
}
