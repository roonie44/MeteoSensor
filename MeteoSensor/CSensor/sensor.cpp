#include "main.h"

CSensorModule::CSensorModule()
{
   CEvents::Subscribe(this);
}

void CSensorModule::Init(void)
{
   BME280.Init();
}

void CSensorModule::Handle(void)
{
//   if (MeasurementRequest)
//   {
//      MeasurementRequest = false;

      signed int s32Temperature;
      unsigned int u32Humidity;
      unsigned int u32Pressure;

      if (BME280.GetTemperature(s32Temperature) == Status::OK)
         ;

      if (BME280.GetHumidity(u32Humidity) == Status::OK)
         ;

      if (BME280.GetPressure(u32Pressure) == Status::OK)
         ;
//   }
   //HandleRequest = false;

   switch (RequestDataType)
   {
   case TEMPERATURE:
      signed int s32Temperature;
      if (BME280.GetTemperature(s32Temperature) == Status::OK && u32CallbackId != 0)
         BlueNRG.Callback(u32CallbackId, &s32Temperature);
      break;

   case HUMIDITY:
      unsigned int u32Humidity;
      if(BME280.GetHumidity(u32Humidity) == Status::OK && u32CallbackId != 0)
         BlueNRG.Callback(u32CallbackId, &u32Humidity);
      break;

   case PRESSURE:
      unsigned int u32Pressure;
      if(BME280.GetPressure(u32Pressure) == Status::OK && u32CallbackId != 0)
         BlueNRG.Callback(u32CallbackId, &u32Pressure);
      break;

   default:
      break;
   }
}

void CSensorModule::Event()
{
   switch (CEvents::GetEventId())
   {
      case EventId::PeriodicWakeUp:
         HandleRequest(this);
         break;

      default:
         break;
   }
}

void CSensorModule::DataRequest(eDataType DataType, unsigned int u32CallbackId)
{
   this->RequestDataType   = DataType;
   //this->HandleRequest     = true;
   this->u32CallbackId     = u32CallbackId;
}
