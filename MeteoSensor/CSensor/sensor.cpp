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
   signed int s32Temperature;
   if (BME280.GetTemperature(s32Temperature) == Status::OK)
      Data.SetTemperature(s32Temperature);

   unsigned int u32Humidity;
   if (BME280.GetHumidity(u32Humidity) == Status::OK)
      Data.SetHumidity(u32Humidity);

   unsigned int u32Pressure;
   if (BME280.GetPressure(u32Pressure) == Status::OK)
      Data.SetPressure(u32Pressure);
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
   this->u32CallbackId     = u32CallbackId;
}
