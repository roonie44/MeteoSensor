#include "main.h"

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
   Params.u8NameCharLength = strlen(DEVICE_NAME);

   return SendCommand(CMD_OPCODE_GAP_INIT, &Params, sizeof(Params));
}

int CBlueNRGModule::CmdGapSetDiscoverable(void)
{
   TCmdParamsSetDicoverable Params;
            char U8LocalName[strlen(DEVICE_NAME) + 1];
   unsigned char U8Params[sizeof(Params) + sizeof(U8LocalName)];

   if (strlen(DEVICE_NAME) > 0)
   {
      U8LocalName[0]          = 0x09;     // AD_TYPE_COMPLETE_LOCAL_NAME
      strcpy(&U8LocalName[1], DEVICE_NAME);
   }
   else
      U8LocalName[0]          = 0;

   Params.u8AdvertisingType   = 0;        // Connectable undirected advertising (ADV_IND)
   Params.u16IntervalMin      = 0x0800;   // 1.28s
   Params.u16IntervalMax      = 0x0800;   // 1.28s
   Params.u8OwnAddressType    = 1;        // Public Device Address
   Params.u8FilterPolicy      = 0;        //  Allow scan request from any, allow connect request from any
   Params.u8LocalNameLen      = strlen(U8LocalName);
   Params.u8ServiceUUIDLen    = 0;
   Params.u16ConnIntervalMin  = 0xFFFF;   // No specific minimum
   Params.u16ConnIntervalMax  = 0xFFFF;   // No specific maximum

   memcpy(&U8Params[0], &Params, offsetof(TCmdParamsSetDicoverable, u8ServiceUUIDLen));
   memcpy(&U8Params[offsetof(TCmdParamsSetDicoverable, u8ServiceUUIDLen)], U8LocalName, sizeof(U8LocalName));
   memcpy(&U8Params[offsetof(TCmdParamsSetDicoverable, u8ServiceUUIDLen) + sizeof(U8LocalName)], &Params.u8ServiceUUIDLen, sizeof(TCmdParamsSetDicoverable) - offsetof(TCmdParamsSetDicoverable, u8ServiceUUIDLen));
   return SendCommand(CMD_OPCODE_GAP_SET_DISCOVERABLE, U8Params, sizeof(U8Params));
}
