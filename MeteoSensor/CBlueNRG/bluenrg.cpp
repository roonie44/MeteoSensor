#include "main.h"

const unsigned char CBlueNRGModule::SPI_WRITE_HEADER[5]                       = { 0x0A, 0x00, 0x00, 0x00, 0x00, };
const unsigned char CBlueNRGModule::SPI_READ_HEADER[5]                        = { 0x0B, 0x00, 0x00, 0x00, 0x00, };

const unsigned char CBlueNRGModule::SERVICE_UUID_DEVICE_INFORMATION[2]        = { 0x0A, 0x18, };
const unsigned char CBlueNRGModule::CHARACTERISTIC_UUID_HARDWARE_REVISION[2]  = { 0x27, 0x2A, };
const unsigned char CBlueNRGModule::CHARACTERISTIC_UUID_SOFTWARE_REVISION[2]  = { 0x28, 0x2A, };

const unsigned char CBlueNRGModule::SERVICE_UUID_ENVIRONMENTAL_SENSOR[2]      = { 0x1A, 0x18, };
const unsigned char CBlueNRGModule::CHARACTERISTIC_UUID_TEMPERATURE[2]        = { 0x6E, 0x2A, };
const unsigned char CBlueNRGModule::CHARACTERISTIC_UUID_HUMIDITY[2]           = { 0x6F, 0x2A, };

CBlueNRGModule::CBlueNRGModule(void)
{
   CEvents::Subscribe(this);
}

void CBlueNRGModule::Init(void)
{
   // Release reset
   LL_GPIO_SetOutputPin(PIN_BLUENRG_SPI_RESET_PORT, PIN_BLUENRG_SPI_RESET_PIN);
}

void CBlueNRGModule::Handle(void)
{
   if (IsDataToRead())
   {
      HandlePendingData();
   }

   switch (State)
   {
   case STATE_GATT_INIT:
      Log.Str("BLE GATT Init\r");
      CmdGattInit();
      State = STATE_GAP_INIT;
      break;

   case STATE_GAP_INIT:
      Log.Str("BLE GAP Init\r");
      CmdGapInit();
      State = STATE_CHAR_UPDATE_DEVICE_NAME;
      break;

   case STATE_CHAR_UPDATE_DEVICE_NAME:
      Log.Str("BLE char update [DEVICE_NAME]\r");
      CmdGattUpdateCharValue(u16ServiceHandle, u16DeviceNameCharHandle, 0, strlen(Data.GetDeviceName()), Data.GetDeviceName());
      State = STATE_CHAR_UPDATE_APPEARANCE;
      break;

   case STATE_CHAR_UPDATE_APPEARANCE:
      Log.StrHexR("BLE char update [APPEARANCE]: ", APPEARANCE, sizeof(APPEARANCE));
      CmdGattUpdateCharValue(u16ServiceHandle, u16AppearanceCharHandle, 0, sizeof(APPEARANCE), &APPEARANCE);
      State = STATE_SERVICE_ADD_DEVICE_INFORMATION;
      break;

   case STATE_SERVICE_ADD_DEVICE_INFORMATION:
      Log.Str("BLE service add [DEVICE INFORMATION]\r");
      CmdGattAddService(UUID_TYPE_16, SERVICE_UUID_DEVICE_INFORMATION, PRIMARY_SERVICE, 7, &Service.DeviceInformation.u16Handle);
      State = STATE_CHAR_ADD_SOFTWARE_REVISION;
      break;

   case STATE_CHAR_ADD_SOFTWARE_REVISION:
      Log.Str("BLE char add [SOFTWARE REVISION]\r");
      CmdGattAddChar(Service.DeviceInformation.u16Handle, UUID_TYPE_16, CHARACTERISTIC_UUID_SOFTWARE_REVISION, strlen(Data.GetFirmwareVersion()), CHAR_PROP_READ, 0, 0x04, 16, 0, &Service.DeviceInformation.Characteristic.SoftwareRevision.u16Handle, &Service.DeviceInformation.Characteristic.SoftwareRevision.Value.u16Handle);
      State = STATE_CHAR_UPDATE_SOFTWARE_REVISION;
      break;

   case STATE_CHAR_UPDATE_SOFTWARE_REVISION:
      Log.Str("BLE char update [SOFTWARE REVISION]\r");
      CmdGattUpdateCharValue(Service.DeviceInformation.u16Handle, Service.DeviceInformation.Characteristic.SoftwareRevision.u16Handle, 0, strlen(Data.GetFirmwareVersion()), Data.GetFirmwareVersion());
      State = STATE_SERVICE_ADD_ENVIRONMENTAL_SENSOR;
      break;

   case STATE_SERVICE_ADD_ENVIRONMENTAL_SENSOR:
      Log.Str("BLE service add [ENVIRONMENTAL SENSING]\r");
      CmdGattAddService(UUID_TYPE_16, SERVICE_UUID_ENVIRONMENTAL_SENSOR, PRIMARY_SERVICE, 7, &Service.EnvironmentalSensing.u16Handle);
      State = STATE_CHAR_ADD_TEMPERATURE;
      break;

   case STATE_CHAR_ADD_TEMPERATURE:
      Log.Str("BLE char add [TEMPERATURE]\r");
      CmdGattAddChar(Service.EnvironmentalSensing.u16Handle, UUID_TYPE_16, CHARACTERISTIC_UUID_TEMPERATURE, sizeof(signed short), CHAR_PROP_READ, 0, 0x04, 16, 0, &Service.EnvironmentalSensing.Characteristic.Temperature.u16Handle, &Service.EnvironmentalSensing.Characteristic.Temperature.Value.u16Handle);
      State = STATE_CHAR_ADD_HUMIDITY;
      break;

   case STATE_CHAR_ADD_HUMIDITY:
      Log.Str("BLE char add [HUMIDITY]\r");
      CmdGattAddChar(Service.EnvironmentalSensing.u16Handle, UUID_TYPE_16, CHARACTERISTIC_UUID_HUMIDITY, sizeof(unsigned short), CHAR_PROP_READ, 0, 0x04, 16, 0, &Service.EnvironmentalSensing.Characteristic.Humidity.u16Handle, &Service.EnvironmentalSensing.Characteristic.Humidity.Value.u16Handle);
      State = STATE_SET_ADVERTISING_PARAMS;
      break;

   case STATE_SET_ADVERTISING_PARAMS:
      Log.Str("BLE LE Set Advertising Params\r");
      CmdLeSetAdvertisingParams();
      State = STATE_SET_ADVERTISE_ENABLE;
      break;

   case STATE_SET_ADVERTISE_ENABLE:
      Log.Str("BLE LE Set Advertise Enable\r");
      CmdLeSetAdvertiseEnable();
      State = STATE_SET_ADVERTISING_DATA;
      break;

   case STATE_SET_ADVERTISING_DATA:
      Log.Str("BLE LE Set Advertising Data\r");
      CmdLeSetAdvertisingData();
      State = STATE_ADVERTISING;
      break;

   case STATE_ADVERTISING:
      if (bDataUpdate)
      {
         bDataUpdate = false;
         State = STATE_SET_ADVERTISING_DATA;
      }
      break;

   case STATE_CONNECTED:
      if (bDataUpdate == true)
      {
         bDataUpdate = false;

         signed short   s16TemperatureValue  = Data.GetTemperature();
         unsigned short u16HumidityValue     = Data.GetHumidity();

         CmdGattUpdateCharValue(Service.EnvironmentalSensing.u16Handle, Service.EnvironmentalSensing.Characteristic.Temperature.u16Handle,   0, sizeof(s16TemperatureValue),  &s16TemperatureValue);
         CmdGattUpdateCharValue(Service.EnvironmentalSensing.u16Handle, Service.EnvironmentalSensing.Characteristic.Humidity.u16Handle,      0, sizeof(u16HumidityValue),     &u16HumidityValue);

         if (bDataReadRequest == true)
         {
            bDataReadRequest = false;

            CmdGattAllowRead(Connection.u16Handle);
         }
      }
      break;
   }
}

void CBlueNRGModule::Event()
{
   switch (CEvents::GetEventId())
   {
      case EventId::DataUpdateTemperature:
      case EventId::DataUpdateHumidity:
      case EventId::DataUpdatePressure:
         bDataUpdate = true;
         CModule::HandleRequest(this);
         break;

      default:
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

bool CBlueNRGModule::IsDataToRead(void)
{
   return LL_GPIO_IsInputPinSet(PIN_BLUENRG_SPI_IRQ_PORT, PIN_BLUENRG_SPI_IRQ_PIN) ? true : false;
}

int CBlueNRGModule::HandlePendingData(void)
{
   TSpiHeader  SpiHeader;

   while (IsDataToRead())
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

void CBlueNRGModule::InterruptIRQ(void)
{
   HandleRequest(this);
}
