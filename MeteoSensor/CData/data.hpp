#pragma once

class CData
{
   struct
   {
      const char     DeviceName[32]             = "MeteoNode";
      const char     BuildDate[32]              = __DATE__ " " __TIME__ ;
      const char     FirmwareVersion[16]        = "v1.4";
   } Information;

   struct
   {
      unsigned short u16PeriodicWakeUpInterval  = 300;   // s
      unsigned short u16AdvertisingInterval     = 1280;  // ms
      unsigned short u16LedBlinkTime            = 100;   // ms
   } Configuration;

   struct
   {
      signed int     s32Temperature             = __INT32_MAX__;
      unsigned int   u32Humidity                = __UINT32_MAX__;
      bool           bValidity                  = false;
   } Measurements;

public:
#pragma pack(1)
   struct TDeviceReadouts
   {
      struct
      {
         const unsigned char  u8Length = 5;
         const unsigned char  u8Type   = 0x16;
         const unsigned short u16UUID  = 0x2A6F; // Humidity
         unsigned short       u16HumidityValue;
      }ServiceDataHumidity;
      struct
      {
         const unsigned char  u8Length = 5;
         const unsigned char  u8Type   = 0x16;
         const unsigned short u16UUID  = 0x2A6E; // Temperature
         signed short         s16TemperatureValue;
      }ServiceDataTemperature;
   };
#pragma pack()

   const char*    GetDeviceName()            { return Information.DeviceName; }
   const char*    GetBuildDate()             { return Information.BuildDate; }
   const char*    GetFirmwareVersion()       { return Information.FirmwareVersion; }

   unsigned short GetPeriodicWakeUpInterval(){ return Configuration.u16PeriodicWakeUpInterval; }
   unsigned short GetAdvertisingInterval()   { return Configuration.u16AdvertisingInterval; }
   unsigned short GetLedBlinkTime()          { return Configuration.u16LedBlinkTime; }

   bool           IsMeasurementAvailable()   { return Measurements.bValidity; }
   signed int     GetTemperature()           { return Measurements.s32Temperature; }
   unsigned int   GetHumidity()              { return Measurements.u32Humidity; }

   void           SetTemperature(signed int s32Temperature);
   void           SetHumidity(unsigned int u32Humidity);

private:
   void           SetValidity();
};
