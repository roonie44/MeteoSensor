#include "main.h"

void CSensorModule::Init(void)
{
   BME280.Init();
}

void CSensorModule::Handle(void)
{
   HandleRequest = false;

   switch (RequestDataType)
   {
   case TEMPERATURE:
      signed int s32Temperature;
      s32Temperature = BME280.GetTemperature();
      if(u32CallbackId != 0)
         BlueNRG.Callback(u32CallbackId, &s32Temperature);
      break;

   case HUMIDITY:
      unsigned int u32Humidity;
      u32Humidity = BME280.GetHumidity();
      if(u32CallbackId != 0)
         BlueNRG.Callback(u32CallbackId, &u32Humidity);
      break;

   case PRESSURE:
      unsigned int u32Pressure;
      u32Pressure = BME280.GetPressure();
      if(u32CallbackId != 0)
         BlueNRG.Callback(u32CallbackId, &u32Pressure);
      break;

   default:
      break;
   }
}

void CSensorModule::DataRequest(eDataType DataType, unsigned int u32CallbackId)
{
   this->RequestDataType   = DataType;
   this->HandleRequest     = true;
   this->u32CallbackId     = u32CallbackId;
}
