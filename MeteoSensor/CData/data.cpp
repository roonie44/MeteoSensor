#include "data.hpp"
#include "../inc/events.hpp"

CData::CData()
{
   Configuration.u16PeriodicWakeUpInterval   = 15;    // s
   Configuration.u16AdvertisingInterval      = 1280;  // ms
}

void CData::SetTemperature(signed int s32Temperature)
{
   Measurements.s32Temperature = s32Temperature;
   CEvents::Publish(EventId::DataUpdateTemperature);
}

void CData::SetHumidity(unsigned int u32Humidity)
{
   Measurements.u32Humidity = u32Humidity;
   CEvents::Publish(EventId::DataUpdateHumidity);
}

void CData::SetPressure(unsigned int u32Pressure)
{
   Measurements.u32Pressure = u32Pressure;
   CEvents::Publish(EventId::DataUpdatePressure);
}
