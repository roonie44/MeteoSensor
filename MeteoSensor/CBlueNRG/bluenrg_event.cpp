#include "main.h"

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
         TEventParamsDisconnectionComplete *pParams;
         pParams = (TEventParamsDisconnectionComplete*)pEventPacket->U8Param;
         Log.StrHex(" Disconnected: conn ", pParams->u16ConnectionHandle, 2);
         Log.StrHexR(" reason ", pParams->u8Reason, 1);
         EventDisconnectionComplete(pParams->u16ConnectionHandle);
         break;
      }

      case HCI_EVENT_LE_META:
      {
         TEventParamsLeMetaEvent *pParams;
         pParams = (TEventParamsLeMetaEvent*)pEventPacket->U8Param;
         switch(pParams->u8SubeventCode)
         {
            case 0x01:
            {
               TEventParamsMetaConnectionComplete *pParams2;
               pParams2 = (TEventParamsMetaConnectionComplete*)pParams->U8Param;
               Log.StrHex(" Connected: conn ", pParams2->u16ConnectionHandle, 2);
               Log.StrBlobR(" from 0x", pParams2->U8PeerAddress, sizeof(pParams2->U8PeerAddress));
               EventConnected(pParams2->u16ConnectionHandle);
            }
            break;

            case 0x03:
            {
               TEventParamsMetaConnectionUpdate *pParams2;
               pParams2 = (TEventParamsMetaConnectionUpdate*)pParams->U8Param;
               Log.StrHexR(" Updated: conn ", pParams2->u16ConnectionHandle, 2);
            }
            break;

            default:
               Log.StrBlobR(" META other ", (unsigned char*)pEventPacket, s32EventPacketLen);

         }

         break;
      }

      case HCI_EVENT_VENDOR:
      {
         TEventVSParams *pParams;
         pParams = (TEventVSParams*)pEventPacket->U8Param;
         switch(pParams->u16EventCode)
         {
         case EVT_BLUE_INITIALIZED:
            Log.StrHexR(" BlueNRG init code ", ((TEventParamsBlueInitialized*)pParams->U8Param)->u8ReasonCode, 1);
            break;

         case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
         {
            TEventParamsBlueGattAttributeModified *pParams2;
            pParams2 = (TEventParamsBlueGattAttributeModified*)pParams->U8Param;
            Log.StrHex(" Attribute ", pParams2->u16AttributeHandle, 2);
            Log.StrBlobR(" value modified: ", pParams2->U8Value, pParams2->u8DataLength);
            break;
         }

         case EVT_BLUE_GATT_READ_PERMIT_REQUEST:
         {
            TEventParamsBlueGattReadPermitReqest *pParams2;
            pParams2 = (TEventParamsBlueGattReadPermitReqest*)pParams->U8Param;
            Log.StrHex(" Attribute ", pParams2->u16AttributeHandle, 2);
            Log.StrHexR(" read requested by conn ", pParams2->u16ConnectionHandle, 2);
            EventReadPermitRequest(pParams2->u16AttributeHandle);
            break;
         }

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

      case CMD_OPCODE_GATT_ADD_SERVICE:
      {
         TCmdRespGattAddService *pResp = (TCmdRespGattAddService*)pReturn;
         if(pResp->u8Status == 0 && p16ServiceHandle != NULL)
         {
            *p16ServiceHandle = pResp->u16ServiceHandle;
            p16ServiceHandle  = NULL;
         }
         break;
      }

      case CMD_OPCODE_GATT_ADD_CHARACTERISTIC:
      {
         TCmdRespGattAddChar *pResp = (TCmdRespGattAddChar*)pReturn;
         if(pResp->u8Status == 0)
         {
            if(p16CharHandle != NULL)
            {
               *p16CharHandle       = pResp->u16CharHandle;
               p16CharHandle        = NULL;
            }
            if(p16CharValueHandle != NULL)
            {
               *p16CharValueHandle  = pResp->u16CharHandle + 1; // Nie pytaj czemu
               p16CharValueHandle   = NULL;
            }
         }
         break;
      }
   }
}

void CBlueNRGModule::EventReadPermitRequest(unsigned short u16AttributeHandle)
{
   if (u16AttributeHandle == Service.EnvironmentalSensing.Characteristic.Temperature.Value.u16Handle)
   {
      Sensor.DataRequest(CSensorModule::eDataType::TEMPERATURE, u16AttributeHandle);
      Service.EnvironmentalSensing.Characteristic.Temperature.RequestRead  = true;
      Service.EnvironmentalSensing.Characteristic.Temperature.Updated      = false;
   }
   else if (u16AttributeHandle == Service.EnvironmentalSensing.Characteristic.Humidity.Value.u16Handle)
   {
      Sensor.DataRequest(CSensorModule::eDataType::HUMIDITY, u16AttributeHandle);
      Service.EnvironmentalSensing.Characteristic.Humidity.RequestRead     = true;
      Service.EnvironmentalSensing.Characteristic.Humidity.Updated         = false;
   }
   else if (u16AttributeHandle == Service.EnvironmentalSensing.Characteristic.Pressure.Value.u16Handle)
   {
      Sensor.DataRequest(CSensorModule::eDataType::PRESSURE, u16AttributeHandle);
      Service.EnvironmentalSensing.Characteristic.Pressure.RequestRead     = true;
      Service.EnvironmentalSensing.Characteristic.Pressure.Updated         = false;
   }
   //else return;
   else CmdGattAllowRead(Connection.u16Handle);

   State = STATE_UPDATE_DATA;
}

void CBlueNRGModule::EventConnected(unsigned short u16ConnectionHandle)
{
   Connection.u16Handle = u16ConnectionHandle;
   State = STATE_CONNECTED;
   LL_GPIO_SetOutputPin(PIN_LED_GREEN_PORT, PIN_LED_GREEN_PIN);
}

void CBlueNRGModule::EventDisconnectionComplete(unsigned short u16ConnectionHandle)
{
   Connection.u16Handle = 0;
   State = STATE_SET_DISCOVERABLE;
   LL_GPIO_ResetOutputPin(PIN_LED_GREEN_PORT, PIN_LED_GREEN_PIN);
}
