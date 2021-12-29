#include "main.h"
#include "string.h"

int CBlueNRGModule::SendCommand(unsigned short u16OPCode, void* pParams, int s32ParamsLen)
{
   unsigned char     U8CmdPacket[sizeof(TCommandPacket) + s32ParamsLen];
   TCommandPacket*   pCommandPacket = (TCommandPacket*)U8CmdPacket;

   pCommandPacket->u8PacketType  = HCI_PACKET_COMMAND;
   pCommandPacket->u16OPCode     = u16OPCode;
   pCommandPacket->u8ParamsLen   = s32ParamsLen;
   memcpy(pCommandPacket->U8Param, pParams, s32ParamsLen);

   return SendPacketViaSpi(U8CmdPacket, sizeof(U8CmdPacket));
}

int CBlueNRGModule::CmdWriteConfigData(unsigned char u8Offset, unsigned char u8Length, void* pData)
{
   unsigned char              U8Params[sizeof(TCmdParamsWriteConfigData) + u8Length];
   TCmdParamsWriteConfigData* pParams = (TCmdParamsWriteConfigData*)U8Params;

   pParams->u8Offset = u8Offset;
   pParams->u8Length = u8Length;
   memcpy(pParams->U8Data, pData, u8Length);

   return SendCommand(CMD_OPCODE_WRITE_CONFIG_DATA, U8Params, sizeof(U8Params));
}

int CBlueNRGModule::CmdLeSetAdvertisingParams(void)
{
   TCmdParamsSetAdvertisingParameters  Params;

   signed int s32AdvertisingIntervalMin = Data.GetAdvertisingInterval() - 50;
   signed int s32AdvertisingIntervalMax = Data.GetAdvertisingInterval() + 50;
   s32AdvertisingIntervalMin = (s32AdvertisingIntervalMin * 1000) / 625; // 1 ms -> 625 us, range 0x0020 to 0x4000
   s32AdvertisingIntervalMax = (s32AdvertisingIntervalMax * 1000) / 625; // 1 ms -> 625 us, range 0x0020 to 0x4000

   Params.u16IntervalMin         = MIN(MAX(s32AdvertisingIntervalMin, 0x20), 0x4000);
   Params.u16IntervalMax         = MIN(MAX(s32AdvertisingIntervalMax, 0x20), 0x4000);
   Params.u8AdvertisingType      = 0;        // Connectable undirected advertising (ADV_IND)
   Params.u8OwnAddressType       = 1;        // Random Device Address
   Params.u8DirectAddressType    = 1;        // Random Device Address
   memset(Params.U8DirectAddress, 0xFF, sizeof(Params.U8DirectAddress));
   Params.u8ChannelMap           = 0x07;     // All channels used
   Params.u8FilterPolicy         = 0;        // Allow scan request from any, allow connect request from any

   return SendCommand(CMD_OPCODE_LE_SET_ADVERTISING_PARAMETERS, &Params, sizeof(Params));
}

int CBlueNRGModule::CmdLeSetAdvertisingData(void)
{
   TCmdParamsSetAdvertisingData  Params = { 0, };

   char U8LocalName[strlen(Data.GetDeviceName()) + 2];

   U8LocalName[0]          = strlen(Data.GetDeviceName()) + 1;
   U8LocalName[1]          = 0x09;     // AD_TYPE_COMPLETE_LOCAL_NAME
   if (strlen(Data.GetDeviceName()) > 0)
      strcpy(&U8LocalName[2], Data.GetDeviceName());
   memcpy(Params.U8AdvertisingData + Params.u8AdvertisingDataLen, U8LocalName, sizeof(U8LocalName));
   Params.u8AdvertisingDataLen += sizeof(U8LocalName);

   unsigned char U8Flags[] = { 2, 0x01, 0x02, };

   memcpy(Params.U8AdvertisingData + Params.u8AdvertisingDataLen, U8Flags, sizeof(U8Flags));
   Params.u8AdvertisingDataLen += sizeof(U8Flags);

   if (Data.IsMeasurementAvailable() == true)
   {
      CData::TDeviceReadouts  DeviceReaouts;

      DeviceReaouts.ServiceDataTemperature.s16TemperatureValue = Data.GetTemperature();
      DeviceReaouts.ServiceDataHumidity.u16HumidityValue       = Data.GetHumidity();
      memcpy(Params.U8AdvertisingData + Params.u8AdvertisingDataLen, &DeviceReaouts, sizeof(DeviceReaouts));
      Params.u8AdvertisingDataLen += sizeof(DeviceReaouts);
   }
   else
      Log.Str("BLE Measurement data unavailable\r");

   return SendCommand(CMD_OPCODE_LE_SET_ADVERTISING_DATA, &Params, sizeof(Params));
}

int CBlueNRGModule::CmdLeSetScanResponseData(void)
{
   TCmdParamsSetScanRespData  Params = { 0, };

   return SendCommand(CMD_OPCODE_LE_SET_SCAN_RESP_DATA, &Params, sizeof(Params));
}

int CBlueNRGModule::CmdLeSetAdvertiseEnable(void)
{
   TCmdParamsSetAdvertiseEnable  Params;

   Params.u8AdvertisingEnable    = true;

   return SendCommand(CMD_OPCODE_LE_SET_ADVERTISE_ENABLE, &Params, sizeof(Params));
}

int CBlueNRGModule::CmdGattInit(void)
{
   return SendCommand(CMD_OPCODE_GATT_INIT, NULL, 0);
}

int CBlueNRGModule::CmdGattAddService(unsigned char u8UUIDType, const unsigned char* U8UUID, unsigned char u8ServiceType, unsigned char u8MaxAttributeRecords, unsigned short* p16ServiceHandle)
{
   this->p16ServiceHandle = p16ServiceHandle;

   if(u8UUIDType == UUID_TYPE_128)
   {
      TCmdParamsGattAddServiceUUID128  Params;

      Params.u8UUIDType             = u8UUIDType;
      Params.u8ServiceType          = u8ServiceType;
      Params.u8MaxAttributeRecords  = u8MaxAttributeRecords;
      memcpy(Params.U8UUID, U8UUID, 16);

      return SendCommand(CMD_OPCODE_GATT_ADD_SERVICE, &Params, sizeof(Params));
   }
   else if(u8UUIDType == UUID_TYPE_16)
   {
      TCmdParamsGattAddServiceUUID16  Params;

      Params.u8UUIDType             = u8UUIDType;
      Params.u8ServiceType          = u8ServiceType;
      Params.u8MaxAttributeRecords  = u8MaxAttributeRecords;
      memcpy(Params.U8UUID, U8UUID, 2);

      return SendCommand(CMD_OPCODE_GATT_ADD_SERVICE, &Params, sizeof(Params));
   }
   else return 0;
}

int CBlueNRGModule::CmdGattAddChar(unsigned short u16ServiceHandle, unsigned char u8UUIDType, const unsigned char* U8UUID, unsigned char u8ValueLength, unsigned char u8Properties, unsigned char u8Permissions, unsigned char u8EventMask, unsigned char u8EncryptKeySize, unsigned char u8ValueVarLen, unsigned short* p16CharHandle, unsigned short* p16CharValueHandle)
{
   this->p16CharHandle              = p16CharHandle;
   this->p16CharValueHandle         = p16CharValueHandle;

   if(u8UUIDType == UUID_TYPE_128)
   {
      TCmdParamsGattAddCharUUID128 Params;

      Params.u16ServiceHandle       = u16ServiceHandle;
      Params.u8UUIDType             = u8UUIDType;
      Params.u8ValueLength          = u8ValueLength;
      Params.u8Properties           = u8Properties;
      Params.u8Permissions          = u8Permissions;
      Params.u8EventMask            = u8EventMask;
      Params.u8EncryptKeySize       = u8EncryptKeySize;
      Params.u8ValueVarLen          = u8ValueVarLen;
      memcpy(Params.U8UUID, U8UUID, 16);

      return SendCommand(CMD_OPCODE_GATT_ADD_CHARACTERISTIC, &Params, sizeof(Params));
   }
   else if(u8UUIDType == UUID_TYPE_16)
   {
      TCmdParamsGattAddCharUUID16 Params;

      Params.u16ServiceHandle       = u16ServiceHandle;
      Params.u8UUIDType             = u8UUIDType;
      Params.u8ValueLength          = u8ValueLength;
      Params.u8Properties           = u8Properties;
      Params.u8Permissions          = u8Permissions;
      Params.u8EventMask            = u8EventMask;
      Params.u8EncryptKeySize       = u8EncryptKeySize;
      Params.u8ValueVarLen          = u8ValueVarLen;
      memcpy(Params.U8UUID, U8UUID, 2);

      return SendCommand(CMD_OPCODE_GATT_ADD_CHARACTERISTIC, &Params, sizeof(Params));
   }
   else return 0;
}

int CBlueNRGModule::CmdGattUpdateCharValue(unsigned short u16ServiceHandle, unsigned short u16CharacteristicHandle, unsigned char u8ValueOffset, unsigned char u8ValueLength, const void* pValue)
{
   unsigned char U8Params[sizeof(TCmdParamsGattUpdateChar) + u8ValueLength];
   TCmdParamsGattUpdateChar* pParams = (TCmdParamsGattUpdateChar*)U8Params;

   pParams->u16ServiceHandle           = u16ServiceHandle;
   pParams->u16CharacteristicHandle    = u16CharacteristicHandle;
   pParams->u8ValueOffset              = u8ValueOffset;
   pParams->u8ValueLength              = u8ValueLength;
   memcpy(pParams->U8Value, pValue, u8ValueLength);

   return SendCommand(CMD_OPCODE_GATT_UPDATE_CHAR_VALUE, U8Params, sizeof(U8Params));
}

int CBlueNRGModule::CmdGattAllowRead(unsigned short u16ConnectionHandle)
{
   TCmdParamsGattAllowRead Params;

   Params.u16ConnectionHandle = u16ConnectionHandle;

   return SendCommand(CMD_OPCODE_GATT_ALLOW_READ, &Params, sizeof(Params));
}

int CBlueNRGModule::CmdGapInit(void)
{
   TCmdParamsGapInit Params;

   Params.u8Role           = 0x01;  // Peripherial
   Params.u8PrivacyEnabled = 0;
   Params.u8NameCharLength = strlen(Data.GetDeviceName());

   return SendCommand(CMD_OPCODE_GAP_INIT, &Params, sizeof(Params));
}
