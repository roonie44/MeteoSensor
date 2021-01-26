#pragma once

#include "module.hpp"
#include "bluenrg_types.h"
#include "bluenrg_command.h"
#include "bluenrg_event.h"

class CBlueNRGModule : public CModule
{
   static const unsigned char SPI_WRITE_HEADER[5];
   static const unsigned char SPI_READ_HEADER[5];

   static const unsigned char SERVICE_UUID_DEVICE_INFORMATION[2];
   static const unsigned char CHARACTERISTIC_UUID_HARDWARE_REVISION[2];
   static const unsigned char CHARACTERISTIC_UUID_SOFTWARE_REVISION[2];

   static const unsigned char SERVICE_UUID_ENVIRONMENTAL_SENSOR[2];
   static const unsigned char CHARACTERISTIC_UUID_PRESSURE[2];
   static const unsigned char CHARACTERISTIC_UUID_TEMPERATURE[2];
   static const unsigned char CHARACTERISTIC_UUID_HUMIDITY[2];

   const unsigned short       APPEARANCE = 1344; // Generic sensor

   enum eState
   {
      STATE_GATT_INIT,
      STATE_GAP_INIT,
      STATE_CHAR_UPDATE_DEVICE_NAME,
      STATE_CHAR_UPDATE_APPEARANCE,
      STATE_SERVICE_ADD_DEVICE_INFORMATION,
      STATE_CHAR_ADD_SOFTWARE_REVISION,
      STATE_CHAR_UPDATE_SOFTWARE_REVISION,
      STATE_SERVICE_ADD_ENVIRONMENTAL_SENSOR,
      STATE_CHAR_ADD_TEMPERATURE,
      STATE_CHAR_ADD_HUMIDITY,
      STATE_CHAR_ADD_PRESSURE,
      STATE_SET_DISCOVERABLE,
      STATE_SET_ADVERTISING_PARAMS,
      STATE_SET_ADVERTISING_DATA,
      STATE_SET_SCAN_RESPONSE_DATA,
      STATE_SET_ADVERTISE_ENABLE,
      STATE_ADVERTISING,
      STATE_CONNECTED,
      STATE_UPDATE_DATA,
   }State;

   unsigned short *p16ServiceHandle;
   unsigned short *p16CharHandle;
   unsigned short *p16CharValueHandle;

   unsigned short u16ServiceHandle;
   unsigned short u16DeviceNameCharHandle;
   unsigned short u16AppearanceCharHandle;

   struct
   {
      struct
      {
         unsigned short u16Handle;
         struct
         {
            struct
            {
               unsigned short u16Handle;
               struct
               {
                  unsigned short u16Handle;
               }Value;
            }SoftwareRevision;
         }Characteristic;
      }DeviceInformation;
      struct
      {
         unsigned short u16Handle;
         struct
         {
            struct
            {
               unsigned short u16Handle;
               struct
               {
                  unsigned short u16Handle;
                  signed short   s16Value;      // 0,01 [�C]
               }Value;
               bool           RequestRead;
               bool           Updated;
            }Temperature;
            struct
            {
               unsigned short u16Handle;
               struct
               {
                  unsigned short u16Handle;
                  unsigned short u16Value;      // 0,01 [%]
               }Value;
               bool           RequestRead;
               bool           Updated;
            }Humidity;
            struct
            {
               unsigned short u16Handle;
               struct
               {
                  unsigned short u16Handle;
                  unsigned int   u32Value;      // 0.1 [Pa]
               }Value;
               bool           RequestRead;
               bool           Updated;
            }Pressure;
         }Characteristic;
      }EnvironmentalSensing;
   }Service;

   struct
   {
      unsigned short u16Handle;
   }Connection;

   bool bDataUpdate;
   bool bRequestRead;

   int  SendPacketViaSpi            (unsigned char* U8Packet, int s32PacketLen);
   int  SendCommand                 (unsigned short u16OPCode, void* pParams, int s32ParamsLen);

   int  CmdWriteConfigData          (unsigned char u8Offset, unsigned char u8Length, void* pData);
   int  CmdLeSetAdvertisingParams   (void);
   int  CmdLeSetAdvertisingData     (void);
   int  CmdLeSetScanResponseData    (void);
   int  CmdLeSetAdvertiseEnable     (void);
   int  CmdGattInit                 (void);
   int  CmdGattAddService           (unsigned char u8UUIDType, const unsigned char* U8UUID, unsigned char u8ServiceType, unsigned char u8MaxAttributeRecords, unsigned short* p16ServiceHandle);
   int  CmdGattAddChar              (unsigned short u16ServiceHandle, unsigned char u8UUIDType, const unsigned char* U8UUID, unsigned char u8ValueLength, unsigned char u8Properties, unsigned char u8Permissions, unsigned char u8EventMask, unsigned char u8EncryptKeySize, unsigned char u8ValueVarLen, unsigned short* p16CharHandle, unsigned short* p16CharValueHandle);
   int  CmdGattUpdateCharValue      (unsigned short u16ServiceHandle, unsigned short u16CharacteristicHandle, unsigned char u8ValueOffset, unsigned char u8ValueLength, const void* pValue);
   int  CmdGattAllowRead            (unsigned short u16ConnectionHandle);
   int  CmdGapInit                  (void);
   int  CmdGapSetDiscoverable       (void);

   bool IsDataToRead                (void);
   int  HandlePendingData           (void);
   void ParseEvent                  (TEventPacket* pEventPacket, int s32EventPacketLen);
   void EventCommandComplete        (unsigned short u16OPCode, void* pReturn, int s32ReturnLen);
   void EventReadPermitRequest      (unsigned short u16AttributeHandle);
   void EventConnected              (unsigned short u16ConnectionHandle);
   void EventDisconnectionComplete  (unsigned short u16ConnectionHandle);


public:
        CBlueNRGModule              (void);
   void Init                        (void);
   void Handle                      (void);
   void Event                       ();
   void InterruptIRQ                (void);
};
