#include "data.hpp"
#include "../inc/events.hpp"

void CData::SetTemperature(signed int s32Temperature)
{
   Measurements.s32Temperature = s32Temperature;
   SetValidity();
   CEvents::Publish(EventId::DataUpdateTemperature);
}

void CData::SetHumidity(unsigned int u32Humidity)
{
   Measurements.u32Humidity = u32Humidity;
   SetValidity();
   CEvents::Publish(EventId::DataUpdateHumidity);
}

void CData::SetValidity()
{
   if (Measurements.s32Temperature == __INT32_MAX__)
   {
      Measurements.bValidity = false;
      return;
   }
   if (Measurements.u32Humidity == __UINT32_MAX__)
   {
      Measurements.bValidity = false;
      return;
   }
   Measurements.bValidity = true;
}
