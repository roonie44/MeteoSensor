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
#pragma pack(1)
   struct TScanResponseData
   {
      struct
      {
         const unsigned char  u8Length = 5;
         const unsigned char  u8Type   = 0x16;
         const unsigned short u16UUID  = 0x2A6E; // Temperature
         signed short         s16TemperatureValue;
      }ServiceData;
      struct
      {
         const unsigned char  u8Length = 9;
         const unsigned char  u8Type   = 0xFF;
         const unsigned short u16CompanyIdentifierCode = 0xFFFF; // used in the internal and interoperability tests before a Company ID has been assigned
         unsigned short       u16HumidityValue;
         unsigned int         u32PressureValue;
      }ManufacturerData;
   };
#pragma pack()

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
