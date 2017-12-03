#ifndef BLUENRG_TYPES_H_
#define BLUENRG_TYPES_H_

struct TSpiHeader
{
   unsigned char  u8ReadyInd;
   unsigned char  u8WriteSize;
   unsigned char  u8Dummy1;
   unsigned char  u8ReadSize;
   unsigned char  u8Dummy2;
};

enum eHCI_PacketType
{
   HCI_PACKET_COMMAND            = 0x01,
   HCI_PACKET_ACL_DATA           = 0x02,
   HCI_PACKET_EVENT              = 0x04,
};

enum eHCI_EventType
{
   HCI_EVENT_DISCONNECT_COMPLETE = 0x05,
   HCI_EVENT_CMD_COMPLETE        = 0x0E,
   HCI_EVENT_CMD_STATUS          = 0x0F,
   HCI_EVENT_LE_META             = 0x3E,
   HCI_EVENT_VENDOR              = 0xFF,
};

enum eUUID_Type
{
   UUID_TYPE_16                  = 0x01,
   UUID_TYPE_128                 = 0x02,
};

enum eServiceType
{
   PRIMARY_SERVICE               = 0x01,
   SECONDARY_SERVICE             = 0x02,
};

enum eCharacteristicProperties
{
   CHAR_PROP_BROADCAST           = 0x01,
   CHAR_PROP_READ                = 0x02,
   CHAR_PROP_WRITE_WITHOUT_RESP  = 0x04,
   CHAR_PROP_WRITE               = 0x08,
   CHAR_PROP_NOTIFY              = 0x10,
   CHAR_PROP_INDICATE            = 0x20,
   CHAR_PROP_SIGNED_WRITE        = 0x40,
   CHAR_PROP_EXT                 = 0x80,
};

#pragma pack(1)
struct TCommandPacket
{
   unsigned char  u8PacketType;
   unsigned short u16OPCode;
   unsigned char  u8ParamsLen;
   unsigned char  U8Param[0];
};

struct TEventPacket
{
   unsigned char  u8PacketType;
   unsigned char  u8EventCode;
   unsigned char  u8ParamLength;
   unsigned char  U8Param[0];
};

struct TEventCmdStatusParams
{
   unsigned char  u8Status;
   unsigned char  u8NCmd;
   unsigned short u16OPCode;
};

struct TEventCmdCompleteParams
{
   unsigned char  u8NCmd;
   unsigned short u16OPCode;
   unsigned char  U8Return[0];
};

#define EVT_CODE_BLUE_INITIALIZED      0x0001
struct TEventVendorParams
{
   unsigned short u16EventCode;
   unsigned char  u8ReasonCode;
};
#pragma pack()



#endif /* BLUENRG_TYPES_H_ */
