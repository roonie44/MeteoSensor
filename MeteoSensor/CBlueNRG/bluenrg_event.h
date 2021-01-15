#pragma once

#pragma pack(1)
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

struct TEventParamsDisconnectionComplete
{
   unsigned char  u8Status;
   unsigned short u16ConnectionHandle;
   unsigned char  u8Reason;
};

/******************************************************************************/

struct TEventParamsLeMetaEvent
{
   unsigned char  u8SubeventCode;
   unsigned char  U8Param[0];
};

struct TEventParamsMetaConnectionComplete
{
   unsigned char  u8Status;
   unsigned short u16ConnectionHandle;
   unsigned char  u8Role;
   unsigned char  u8PeerAddressType;
   unsigned char  U8PeerAddress[6];
   unsigned short u16ConnectionInterval;
   unsigned short u16ConnectionLatency;
   unsigned short u16SupervisionTimeout;
   unsigned char  u8MasterClockLatency;
};

struct TEventParamsMetaConnectionUpdate
{
   unsigned char  u8Status;
   unsigned short u16ConnectionHandle;
   unsigned short u16ConnectionInterval;
   unsigned short u16ConnectionLatency;
   unsigned short u16SupervisionTimeout;
};

/******************************************************************************/

struct TEventVSParams
{
   unsigned short u16EventCode;
   unsigned char  U8Param[0];
};

#define EVT_BLUE_INITIALIZED              0x0001
struct TEventParamsBlueInitialized
{
   unsigned char  u8ReasonCode;
};

#define EVT_BLUE_GATT_ATTRIBUTE_MODIFIED  0x0C01
struct TEventParamsBlueGattAttributeModified
{
   unsigned short u16ConnectionHandle;
   unsigned short u16AttributeHandle;
   unsigned char  u8DataLength;
   unsigned short u16Offset;
   unsigned char  U8Value[0];
};

#define EVT_BLUE_GATT_READ_PERMIT_REQUEST  0x0C14
struct TEventParamsBlueGattReadPermitReqest
{
   unsigned short u16ConnectionHandle;
   unsigned short u16AttributeHandle;
   unsigned char  u8DataLength;
   unsigned short u16Offset;
};
#pragma pack()
