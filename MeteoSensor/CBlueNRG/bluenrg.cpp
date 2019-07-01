#include "main.h"

const unsigned char CBlueNRGModule::SPI_WRITE_HEADER[5]                    = { 0x0A, 0x00, 0x00, 0x00, 0x00, };
const unsigned char CBlueNRGModule::SPI_READ_HEADER[5]                     = { 0x0B, 0x00, 0x00, 0x00, 0x00, };

const unsigned char CBlueNRGModule::SERVICE_UUID_ENVIRONMENTAL_SENSOR[2]   = { 0x1A, 0x18, };
const unsigned char CBlueNRGModule::CHARACTERISTIC_UUID_PRESSURE[2]        = { 0x6D, 0x2A, };
const unsigned char CBlueNRGModule::CHARACTERISTIC_UUID_TEMPERATURE[2]     = { 0x6E, 0x2A, };
const unsigned char CBlueNRGModule::CHARACTERISTIC_UUID_HUMIDITY[2]        = { 0x6F, 0x2A, };

void CBlueNRGModule::Init(void)
{
   // Release reset
   LL_GPIO_SetOutputPin(PIN_BLUENRG_SPI_RESET_PORT, PIN_BLUENRG_SPI_RESET_PIN);
   Power.SleepDeny();
}

void CBlueNRGModule::Handle(void)
{
   HandleRequest = false;
   Power.SleepAllow();

   if(DataToRead())
   {
      HandlePendingData();
   }

   switch(State)
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
      Log.Str("BLE char update [DEVICE_NAME]: ");
      Log.Str(DEVICE_NAME);
      Log.Str("\r");
      CmdGattUpdateCharValue(u16ServiceHandle, u16DeviceNameCharHandle, 0, strlen(DEVICE_NAME), DEVICE_NAME);
      State = STATE_CHAR_UPDATE_APPEARANCE;
      break;

   case STATE_CHAR_UPDATE_APPEARANCE:
      Log.StrHexR("BLE char update [APPEARANCE]: ", APPEARANCE, sizeof(APPEARANCE));
      CmdGattUpdateCharValue(u16ServiceHandle, u16AppearanceCharHandle, 0, sizeof(APPEARANCE), &APPEARANCE);
      State = STATE_SERVICE_ADD_ENVIRONMENTAL_SENSOR;
      //State = STATE_SET_DISCOVERABLE;
      break;

   case STATE_SERVICE_ADD_ENVIRONMENTAL_SENSOR:
      Log.Str("BLE service add [ENVIRONMENTAL SENSING]\r");
      CmdGattAddService(UUID_TYPE_16, SERVICE_UUID_ENVIRONMENTAL_SENSOR, PRIMARY_SERVICE, 7, &Service.EnvironmentalSensing.u16Handle);
      State = STATE_CHAR_ADD_TEMPERATURE;
      break;

   case STATE_CHAR_ADD_TEMPERATURE:
      Log.Str("BLE char add [TEMPERATURE]\r");
      CmdGattAddChar(Service.EnvironmentalSensing.u16Handle, UUID_TYPE_16, CHARACTERISTIC_UUID_TEMPERATURE, 2, CHAR_PROP_READ, 0, 0x04, 16, 0, &Service.EnvironmentalSensing.Characteristic.Temperature.u16Handle, &Service.EnvironmentalSensing.Characteristic.Temperature.Value.u16Handle);
      State = STATE_CHAR_ADD_HUMIDITY;
      break;

   case STATE_CHAR_ADD_HUMIDITY:
      Log.Str("BLE char add [HUMIDITY]\r");
      CmdGattAddChar(Service.EnvironmentalSensing.u16Handle, UUID_TYPE_16, CHARACTERISTIC_UUID_HUMIDITY, 2, CHAR_PROP_READ, 0, 0x04, 16, 0, &Service.EnvironmentalSensing.Characteristic.Humidity.u16Handle, &Service.EnvironmentalSensing.Characteristic.Humidity.Value.u16Handle);
      State = STATE_CHAR_ADD_PRESSURE;
      break;

   case STATE_CHAR_ADD_PRESSURE:
      Log.Str("BLE char add [PRESSURE]\r");
      CmdGattAddChar(Service.EnvironmentalSensing.u16Handle, UUID_TYPE_16, CHARACTERISTIC_UUID_PRESSURE, 4, CHAR_PROP_READ, 0, 0x04, 16, 0, &Service.EnvironmentalSensing.Characteristic.Pressure.u16Handle, &Service.EnvironmentalSensing.Characteristic.Pressure.Value.u16Handle);
      State = STATE_SET_DISCOVERABLE;
      //State = STATE_ADVERTISING;
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

   case STATE_UPDATE_DATA:
      if(Service.EnvironmentalSensing.Characteristic.Temperature.RequestRead == true && Service.EnvironmentalSensing.Characteristic.Temperature.Updated == false)
         break;
      if(Service.EnvironmentalSensing.Characteristic.Humidity.RequestRead == true && Service.EnvironmentalSensing.Characteristic.Humidity.Updated == false)
         break;
      if(Service.EnvironmentalSensing.Characteristic.Pressure.RequestRead == true && Service.EnvironmentalSensing.Characteristic.Pressure.Updated == false)
         break;
      CmdGattAllowRead(Connection.u16Handle);

      Service.EnvironmentalSensing.Characteristic.Temperature.RequestRead  = false;
      Service.EnvironmentalSensing.Characteristic.Humidity.RequestRead     = false;
      Service.EnvironmentalSensing.Characteristic.Pressure.RequestRead     = false;
      State = STATE_CONNECTED;
      break;
   }
}

void CBlueNRGModule::Callback(unsigned int u32CallbackId, void *pValue)
{
   if (u32CallbackId == Service.EnvironmentalSensing.Characteristic.Temperature.Value.u16Handle)
   {
      Service.EnvironmentalSensing.Characteristic.Temperature.Value.s16Value  = *(signed short*)pValue;
      CmdGattUpdateCharValue(Service.EnvironmentalSensing.u16Handle, Service.EnvironmentalSensing.Characteristic.Temperature.u16Handle, 0, sizeof(Service.EnvironmentalSensing.Characteristic.Temperature.Value.s16Value), &Service.EnvironmentalSensing.Characteristic.Temperature.Value.s16Value);
      Service.EnvironmentalSensing.Characteristic.Temperature.Updated   = true;
      return;
   }
   if (u32CallbackId == Service.EnvironmentalSensing.Characteristic.Humidity.Value.u16Handle)
   {
      Service.EnvironmentalSensing.Characteristic.Humidity.Value.u16Value  = *(unsigned short*)pValue;
      CmdGattUpdateCharValue(Service.EnvironmentalSensing.u16Handle, Service.EnvironmentalSensing.Characteristic.Humidity.u16Handle, 0, sizeof(Service.EnvironmentalSensing.Characteristic.Humidity.Value.u16Value), &Service.EnvironmentalSensing.Characteristic.Humidity.Value.u16Value);
      Service.EnvironmentalSensing.Characteristic.Humidity.Updated   = true;
      return;
   }
   if (u32CallbackId == Service.EnvironmentalSensing.Characteristic.Pressure.Value.u16Handle)
   {
      Service.EnvironmentalSensing.Characteristic.Pressure.Value.u32Value  = *(unsigned int*)pValue;
      CmdGattUpdateCharValue(Service.EnvironmentalSensing.u16Handle, Service.EnvironmentalSensing.Characteristic.Pressure.u16Handle, 0, sizeof(Service.EnvironmentalSensing.Characteristic.Pressure.Value.u32Value), &Service.EnvironmentalSensing.Characteristic.Pressure.Value.u32Value);
      Service.EnvironmentalSensing.Characteristic.Pressure.Updated   = true;
      return;
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

void CBlueNRGModule::HandleIRQ(void)
{
   this->HandleRequest = true;
   Power.SleepDeny();
}
