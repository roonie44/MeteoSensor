#pragma once

class CData
{
   struct
   {
      char           DeviceName[32] = "MeteoNode";
      char           BuildData[32]  = __DATE__ " " __TIME__ ;
   } Information;

   struct
   {
      unsigned short u16PeriodicWakeUpInterval;
      unsigned short u16AdvertisingInterval;
   } Configuration;

   struct
   {
      signed int     s32Temperature;
      unsigned int   u32Humidity;
      unsigned int   u32Pressure;
   } Measurements;

public:
                  CData();


   unsigned short GetPeriodicWakeUpInterval(){ return Configuration.u16PeriodicWakeUpInterval; }
   unsigned short GetAdvertisingInterval()   { return Configuration.u16AdvertisingInterval; }

   signed int     GetTemperature()           { return Measurements.s32Temperature; }
   unsigned int   GetHumidity()              { return Measurements.u32Humidity; }
   unsigned int   GetPressure()              { return Measurements.u32Pressure; }

   void           SetTemperature(signed int s32Temperature);
   void           SetHumidity(unsigned int u32Humidity);
   void           SetPressure(unsigned int u32Pressure);
};
