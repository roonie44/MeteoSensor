#include "main.h"
#include "init.h"

CLogModule        Log;
CPowerModule      Power;
CSpiModule        Spi;
CBlueNRGModule    BlueNRG;
CI2CModule        I2C(I2C2);
CSensorModule     Sensor;

int main(void)
{
   I2C.pI2C = I2C2;
   Init_Hardware();

   Log.Init();
   Log.Str("\r\r\rApplication: MeteoSensor\r");
   Log.Str("Builddate: ");
   Log.Str(__DATE__);
   Log.Str(", ");
   Log.Str(__TIME__);
   Log.Str("\r");

   Sensor.Init();
   BlueNRG.Init();

   while(1)
   {
      if(BlueNRG.HandleRequest)
         BlueNRG.Handle();

      if(Sensor.HandleRequest)
         Sensor.Handle();

      Power.Handle();
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
