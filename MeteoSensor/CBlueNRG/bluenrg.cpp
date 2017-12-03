#include "main.h"

const unsigned char CBlueNRGModule::PUBLIC_ADDRESS[6]       = { 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0, };
const          char CBlueNRGModule::DEVICE_NAME[]           = "MeteoSensor";

const unsigned char CBlueNRGModule::SPI_WRITE_HEADER[5]     = { 0x0A, 0x00, 0x00, 0x00, 0x00, };
const unsigned char CBlueNRGModule::SPI_READ_HEADER[5]      = { 0x0B, 0x00, 0x00, 0x00, 0x00, };

void CBlueNRGModule::Handle(void)
{
   HandleRequest = false;

   if(DataToRead())
      HandlePendingData();

   switch(State)
   {
   case STATE_SET_PUBLIC_ADDRESS:
      Log.StrBlobR("BLE set public address: ", (unsigned char*)PUBLIC_ADDRESS, sizeof(PUBLIC_ADDRESS));
      CmdWriteConfigData(0x00, sizeof(PUBLIC_ADDRESS), (void*)PUBLIC_ADDRESS);
      State = STATE_GATT_INIT;
      break;

   case STATE_GATT_INIT:
      Log.Str("BLE GATT Init\r");
      CmdGattInit();
      State = STATE_GAP_INIT;
      break;

   case STATE_GAP_INIT:
      Log.Str("BLE GAP Init\r");
      CmdGapInit();
      State = STATE_UPDATE_NAME_CHAR;
      break;

   case STATE_UPDATE_NAME_CHAR:
      Log.Str("BLE Update Name: ");
      Log.Str(DEVICE_NAME);
      Log.Str("\r");
      CmdGattUpdateCharValue(u16ServiceHandle, u16DeviceNameCharHandle, 0, strlen(DEVICE_NAME), DEVICE_NAME);
      State = STATE_SET_DISCOVERABLE;
      break;

   case STATE_SET_DISCOVERABLE:
      Log.Str("BLE GAP Set discoverable\r");
      CmdGapSetDiscoverable();
      State = STATE_ADVERTISING;
      break;

   case STATE_ADVERTISING:
      break;

   case STATE_CONNECTED:
      break;
   }
}

int CBlueNRGModule::SendPacketViaSpi(unsigned char* U8Packet, int s32PacketLen)
{
   TSpiHeader  SpiHeader;

   while(true)
   {
      memcpy(&SpiHeader, SPI_WRITE_HEADER, sizeof(TSpiHeader));
      Spi.EnableCS();
      Spi.ReadWrite((unsigned char*)&SpiHeader, sizeof(TSpiHeader));

      if(SpiHeader.u8ReadyInd != 0x02 || SpiHeader.u8WriteSize < s32PacketLen)
         Spi.DisableCS();
      else break;
   }

   Spi.ReadWrite(U8Packet, s32PacketLen);
   Spi.DisableCS();
   return 0;
}

bool CBlueNRGModule::DataToRead(void)
{
   return LL_GPIO_IsInputPinSet(PIN_BLUENRG_SPI_IRQ_PORT, PIN_BLUENRG_SPI_IRQ_PIN) ? true : false;
}

int CBlueNRGModule::HandlePendingData(void)
{
   TSpiHeader  SpiHeader;

   while(DataToRead())
   {
      memcpy(&SpiHeader, SPI_READ_HEADER, sizeof(TSpiHeader));

      Spi.EnableCS();
      Spi.ReadWrite((unsigned char*)&SpiHeader, sizeof(TSpiHeader));

      if(SpiHeader.u8ReadSize > 0)
      {
         unsigned char U8Data[SpiHeader.u8ReadSize];

         Spi.ReadWrite((unsigned char*)U8Data, SpiHeader.u8ReadSize);

         if(U8Data[0] == HCI_PACKET_EVENT)
            ParseEvent((TEventPacket*)U8Data, SpiHeader.u8ReadSize);
      }

      Spi.DisableCS();
   }
   return 0;
}

void CBlueNRGModule::ParseEvent(TEventPacket* pEventPacket, int s32EventPacketLen)
{
   Log.Str("BLE event");

   switch(pEventPacket->u8EventCode)
   {
      case HCI_EVENT_CMD_STATUS:
      {
         TEventCmdStatusParams *pParams;
         pParams = (TEventCmdStatusParams*)pEventPacket->U8Param;
         Log.StrHex(" CMD ", pParams->u16OPCode, 2);
         Log.StrHexR(" err ", pParams->u8Status, 1);
         break;
      }

      case HCI_EVENT_CMD_COMPLETE:
      {
         TEventCmdCompleteParams *pParams;
         pParams = (TEventCmdCompleteParams*)pEventPacket->U8Param;
         Log.StrHex(" CMD ", pParams->u16OPCode, 2);
         Log.StrBlobR(" ret ", pParams->U8Return, pEventPacket->u8ParamLength - sizeof(TEventCmdCompleteParams));
         EventCommandComplete(pParams->u16OPCode, pParams->U8Return, pEventPacket->u8ParamLength - sizeof(TEventCmdCompleteParams));
         break;
      }

      case HCI_EVENT_DISCONNECT_COMPLETE:
      {
         Log.Str(" Disconnected\r");
         EventDisconnected();
         break;
      }

      case HCI_EVENT_LE_META:
      {
         Log.Str(" Connected\r");
         EventConnected();
         break;
      }

      case HCI_EVENT_VENDOR:
      {
         TEventVendorParams *pParams;
         pParams = (TEventVendorParams*)pEventPacket->U8Param;
         switch(pParams->u16EventCode)
         {
         case EVT_CODE_BLUE_INITIALIZED:
            Log.StrHexR(" BlueNRG init code ", pParams->u8ReasonCode, 1);
            break;

         default:
            Log.StrBlobR(" VENDOR other ", (unsigned char*)pEventPacket, s32EventPacketLen);
            break;
         }
         break;
      }

      default:
         Log.StrBlobR(" UNKOWN ", (unsigned char*)pEventPacket, s32EventPacketLen);
   }
}

void CBlueNRGModule::EventCommandComplete(unsigned short u16OPCode, void* pReturn, int s32ReturnLen)
{
   switch(u16OPCode)
   {
      case CMD_OPCODE_GAP_INIT:
      {
         TCmdRespGapInit *pResp = (TCmdRespGapInit*)pReturn;
         u16ServiceHandle        = pResp->u16ServiceHandle;
         u16DeviceNameCharHandle = pResp->u16DeviceNameCharHandle;
         u16AppearanceCharHandle = pResp->u16AppearanceCharHandle;
         break;
      }
   }
}

void CBlueNRGModule::EventConnected(void)
{
   State = STATE_CONNECTED;
}

void CBlueNRGModule::EventDisconnected(void)
{
   State = STATE_SET_DISCOVERABLE;
}

void CBlueNRGModule::HandleIRQ(void)
{
   this->HandleRequest = true;
}
