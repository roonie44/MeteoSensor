#include "main.h"
#include "init.h"

const          char DEVICE_NAME[32] = "MeteoNode";
const          char BUILD_DATE[32]  = __DATE__ " " __TIME__ ;

CLogModule        Log;
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
   Rtc.SetPeriodicWakeUp(60);
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
      Sensor.DataRequest(CSensorModule::eDataType::TEMPERATURE, 0);
      break;

   case 2:
      Sensor.DataRequest(CSensorModule::eDataType::HUMIDITY, 0);
      break;

   case 3:
      Sensor.DataRequest(CSensorModule::eDataType::PRESSURE, 0);
      break;
   }
}
