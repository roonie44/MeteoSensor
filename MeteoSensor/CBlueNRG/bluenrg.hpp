#ifndef BLUENRG_HPP_
#define BLUENRG_HPP_
#include "bluenrg_types.h"
#include "bluenrg_command.h"

class CBlueNRGModule
{
   const unsigned char  PUBLIC_ADDRESS[6]                      = { 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0, };
   const          char  DEVICE_NAME[12]                        = "MeteoSensor";

   const unsigned char  SPI_WRITE_HEADER[5]                    = { 0x0A, 0x00, 0x00, 0x00, 0x00, };
   const unsigned char  SPI_READ_HEADER[5]                     = { 0x0B, 0x00, 0x00, 0x00, 0x00, };

   const unsigned char  SERVICE_UUID_ENVIRONMENTAL_SENSOR[2]   = { 0x1A, 0x18, };
   const unsigned char  CHARACTERISTIC_UUID_TEMPERATURE[2]     = { 0x6E, 0x2A, };
   const unsigned char  CHARACTERISTIC_UUID_HUMIDITY[2]        = { 0x6F, 0x2A, };

   const unsigned short APPEARANCE = 1344; // Generic sensor

   enum eState
   {
      STATE_SET_PUBLIC_ADDRESS,
      STATE_GATT_INIT,
      STATE_GAP_INIT,
      STATE_CHAR_UPDATE_DEVICE_NAME,
      STATE_CHAR_UPDATE_APPEARANCE,
      STATE_SERVICE_ADD_ENVIRONMENTAL_SENSOR,
      STATE_CHAR_ADD_TEMPERATURE,
      STATE_CHAR_ADD_HUMIDITY,
      STATE_SET_DISCOVERABLE,
      STATE_ADVERTISING,
      STATE_CONNECTED,
   }State;

   unsigned short *p16ServiceHandle;
   unsigned short *p16CharHandle;

   unsigned short u16ServiceHandle;
   unsigned short u16DeviceNameCharHandle;
   unsigned short u16AppearanceCharHandle;

   unsigned short u16ServiceHandleEnvironmentalSensing;
   unsigned short u16CharHandleTemperature;
   unsigned short u16CharHandleHumidity;


   int  SendPacketViaSpi      (unsigned char* U8Packet, int s32PacketLen);
   int  SendCommand           (unsigned short u16OPCode, void* pParams, int s32ParamsLen);

   int  CmdWriteConfigData    (unsigned char u8Offset, unsigned char u8Length, void* pData);
   int  CmdGattInit           (void);
   int  CmdGattAddService     (unsigned char u8UUIDType, const unsigned char* U8UUID, unsigned char u8ServiceType, unsigned char u8MaxAttributeRecords, unsigned short* p16ServiceHandle);
   int  CmdGattAddChar        (unsigned short u16ServiceHandle, unsigned char u8UUIDType, const unsigned char* U8UUID, unsigned char u8ValueLength, unsigned char u8Properties, unsigned char u8Permissions, unsigned char u8EventMask, unsigned char u8EncryptKeySize, unsigned char u8ValueVarLen, unsigned short* p16CharHandle);
   int  CmdGattUpdateCharValue(unsigned short u16ServiceHandle, unsigned short u16CharacteristicHandle, unsigned char u8ValueOffset, unsigned char u8ValueLength, const void* pValue);
   int  CmdGapInit            (void);
   int  CmdGapSetDiscoverable (void);

   bool DataToRead            (void);
   int  HandlePendingData     (void);
   void ParseEvent            (TEventPacket* pEventPacket, int s32EventPacketLen);
   void EventCommandComplete  (unsigned short u16OPCode, void* pReturn, int s32ReturnLen);
   void EventConnected        (void);
   void EventDisconnected     (void);

public:
   bool HandleRequest;

   void Init                  (void);
   void Handle                (void);
   void HandleIRQ             (void);


};



#endif /* BLUENRG_HPP_ */
