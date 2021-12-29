#include "main.h"

CSensorModule::CSensorModule()
{
   CEvents::Subscribe(this);
}

void CSensorModule::Init(void)
{
   HDC1080.Init();

   DataRequest(eDataType::Temperature);
   DataRequest(eDataType::Humidity);
}

void CSensorModule::Handle(void)
{
   HDC1080.PowerOn();

   if (MeasurementRequest.bTemperature)
   {
      MeasurementRequest.bTemperature = false;
      signed int s32Temperature;
      if (HDC1080.GetTemperature(s32Temperature) == Status::OK)
         Data.SetTemperature(s32Temperature);
   }

   if (MeasurementRequest.bHumidity)
   {
      MeasurementRequest.bHumidity = false;
      unsigned int u32Humidity;
      if (HDC1080.GetHumidity(u32Humidity) == Status::OK)
         Data.SetHumidity(u32Humidity);
   }

   HDC1080.PowerOff();
}

void CSensorModule::Event()
{
   switch (CEvents::GetEventId())
   {
      case EventId::PeriodicWakeUp:
         DataRequest(eDataType::Temperature);
         DataRequest(eDataType::Humidity);
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
   }

   HandleRequest(this);
}
