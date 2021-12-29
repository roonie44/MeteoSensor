#include "main.h"
#include "init.h"

CLogModule        Log;
CData             Data;
CPowerModule      Power;
CRTC              Rtc;
CSpiModule        Spi;
CBlueNRGModule    BlueNRG;
CI2C              I2C(I2C2);
CHDC1080          HDC1080(&I2C);
CSensorModule     Sensor;

int main(void)
{
   Init_Hardware();

   Log.Init();
   Log.Str("\r\r\rApplication: ");
   Log.Str(Data.GetDeviceName());
   Log.Str("\rVersion: ");
   Log.Str(Data.GetFirmwareVersion());
   Log.Str("\rBuilddate: ");
   Log.Str(Data.GetBuildDate());
   Log.Str("\r");

   CClock::Init();
   Rtc.SetPeriodicWakeUp(Data.GetPeriodicWakeUpInterval());
   Sensor.Init();
   BlueNRG.Init();

   while(1)
   {
      CModule::NextHandle();
   }
}

void CommandExecute(unsigned int u32CommandCode)
{
   Log.StrHexR("CMD ", u32CommandCode, 1);

   switch(u32CommandCode)
   {
   case 1:
      Sensor.DataRequest(CSensorModule::eDataType::Temperature);
      break;

   case 2:
      Sensor.DataRequest(CSensorModule::eDataType::Humidity);
      break;
   }
}
