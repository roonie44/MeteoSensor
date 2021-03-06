#pragma once

#pragma pack(1)
struct TCommandPacket
{
   unsigned char  u8PacketType;
   unsigned short u16OPCode;
   unsigned char  u8ParamsLen;
   unsigned char  U8Param[0];
};

#define CMD_OPCODE_WRITE_CONFIG_DATA   0xFC0C
struct TCmdParamsWriteConfigData
{
   unsigned char  u8Offset;
   unsigned char  u8Length;
   unsigned char  U8Data[0];
};

/******************************************************************************/

#define CMD_OPCODE_LE_SET_ADVERTISING_PARAMETERS   0x2006
struct TCmdParamsSetAdvertisingParameters
{
   unsigned short u16IntervalMin;
   unsigned short u16IntervalMax;
   unsigned char  u8AdvertisingType;
   unsigned char  u8OwnAddressType;
   unsigned char  u8DirectAddressType;
   unsigned char  U8DirectAddress[6];
   unsigned char  u8ChannelMap;
   unsigned char  u8FilterPolicy;
};
#define CMD_OPCODE_LE_SET_ADVERTISING_DATA         0x2008
struct TCmdParamsSetAdvertisingData
{
   unsigned char  u8AdvertisingDataLen;
   unsigned char  U8AdvertisingData[31];
};
#define CMD_OPCODE_LE_SET_SCAN_RESP_DATA           0x2009
struct TCmdParamsSetScanRespData
{
   unsigned char  u8ScanRespDataLen;
   unsigned char  U8ScanRespData[31];
};
#define CMD_OPCODE_LE_SET_ADVERTISE_ENABLE         0x200A
struct TCmdParamsSetAdvertiseEnable
{
   unsigned char  u8AdvertisingEnable;
};
/******************************************************************************/

#define CMD_OPCODE_GATT_INIT           0xFD01

#define CMD_OPCODE_GATT_ADD_SERVICE    0xFD02
struct TCmdParamsGattAddServiceUUID128
{
   unsigned char u8UUIDType;
   unsigned char U8UUID[16];
   unsigned char u8ServiceType;
   unsigned char u8MaxAttributeRecords;
};
struct TCmdParamsGattAddServiceUUID16
{
   unsigned char u8UUIDType;
   unsigned char U8UUID[2];
   unsigned char u8ServiceType;
   unsigned char u8MaxAttributeRecords;
};
struct TCmdRespGattAddService
{
   unsigned char u8Status;
   unsigned short u16ServiceHandle;
};

#define CMD_OPCODE_GATT_ADD_CHARACTERISTIC   0xFD04
struct TCmdParamsGattAddCharUUID128
{
   unsigned short u16ServiceHandle;
   unsigned char u8UUIDType;
   unsigned char U8UUID[16];
   unsigned char u8ValueLength;
   unsigned char u8Properties;
   unsigned char u8Permissions;
   unsigned char u8EventMask;
   unsigned char u8EncryptKeySize;
   unsigned char u8ValueVarLen;
};
struct TCmdParamsGattAddCharUUID16
{
   unsigned short u16ServiceHandle;
   unsigned char u8UUIDType;
   unsigned char U8UUID[2];
   unsigned char u8ValueLength;
   unsigned char u8Properties;
   unsigned char u8Permissions;
   unsigned char u8EventMask;
   unsigned char u8EncryptKeySize;
   unsigned char u8ValueVarLen;
};
struct TCmdRespGattAddChar
{
   unsigned char u8Status;
   unsigned short u16CharHandle;
};

#define CMD_OPCODE_GATT_UPDATE_CHAR_VALUE    0xFD06
struct TCmdParamsGattUpdateChar
{
   unsigned short u16ServiceHandle;
   unsigned short u16CharacteristicHandle;
   unsigned char  u8ValueOffset;
   unsigned char  u8ValueLength;
   unsigned char* U8Value[0];
};

#define CMD_OPCODE_GATT_ALLOW_READ  0xFD27
struct TCmdParamsGattAllowRead
{
   unsigned short u16ConnectionHandle;
};

/******************************************************************************/

#define CMD_OPCODE_GAP_INIT            0xFC8A
struct TCmdParamsGapInit
{
   unsigned char  u8Role;
   unsigned char  u8PrivacyEnabled;
   unsigned char  u8NameCharLength;
};
struct TCmdRespGapInit
{
   unsigned char u8Status;
   unsigned short u16ServiceHandle;
   unsigned short u16DeviceNameCharHandle;
   unsigned short u16AppearanceCharHandle;
};

#define CMD_OPCODE_GAP_SET_DISCOVERABLE   0xFC83
struct TCmdParamsSetDicoverable
{
   unsigned char  u8AdvertisingType;
   unsigned short u16IntervalMin;
   unsigned short u16IntervalMax;
   unsigned char  u8OwnAddressType;
   unsigned char  u8FilterPolicy;
   unsigned char  u8LocalNameLen;
            char  U8LocalName[0];
   unsigned char  u8ServiceUUIDLen;
   unsigned char  U8ServiceUUIDList[0];
   unsigned short u16ConnIntervalMin;
   unsigned short u16ConnIntervalMax;
};
#pragma pack()
