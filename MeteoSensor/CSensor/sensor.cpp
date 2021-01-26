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
   if (MeasurementRequest.bTemperature)
   {
      MeasurementRequest.bTemperature = false;
      signed int s32Temperature;
      if (BME280.GetTemperature(s32Temperature) == Status::OK)
         Data.SetTemperature(s32Temperature);
   }

   if (MeasurementRequest.bHumidity)
   {
      MeasurementRequest.bHumidity = false;
      unsigned int u32Humidity;
      if (BME280.GetHumidity(u32Humidity) == Status::OK)
         Data.SetHumidity(u32Humidity);
   }

   if (MeasurementRequest.bPressure)
   {
      MeasurementRequest.bPressure = false;
      unsigned int u32Pressure;
      if (BME280.GetPressure(u32Pressure) == Status::OK)
         Data.SetPressure(u32Pressure);
   }
}

void CSensorModule::Event()
{
   switch (CEvents::GetEventId())
   {
      case EventId::PeriodicWakeUp:
         DataRequest(eDataType::Temperature);
         DataRequest(eDataType::Humidity);
         DataRequest(eDataType::Pressure);
         break;

      default:
         break;
   }
}

void CSensorModule::DataRequest(eDataType DataType)
{
   switch (DataType)
   {
      case eDataType::Temperature:
         MeasurementRequest.bTemperature  = true;
         break;

      case eDataType::Humidity:
         MeasurementRequest.bHumidity     = true;
         break;

      case eDataType::Pressure:
         MeasurementRequest.bPressure     = true;
         break;
   }

   HandleRequest(this);
}
