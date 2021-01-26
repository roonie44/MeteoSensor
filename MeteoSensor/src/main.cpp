#include "main.h"
#include "init.h"

const          char DEVICE_NAME[32] = "MeteoNode";
const          char BUILD_DATE[32]  = __DATE__ " " __TIME__ ;

CLogModule        Log;
CData             Data;
CPowerModule      Power;
CRTC              Rtc;
CSpiModule        Spi;
CBlueNRGModule    BlueNRG;
CI2C              I2C(I2C2);
CBME280           BME280(&I2C);
CSensorModule     Sensor;

int main(void)
{
   Init_Hardware();

   Log.Init();
   Log.Str("\r\r\rApplication: MeteoNode\r");
   Log.Str("Builddate: ");
   Log.Str(BUILD_DATE);
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

   case 3:
      Sensor.DataRequest(CSensorModule::eDataType::Pressure);
      break;
   }
}
