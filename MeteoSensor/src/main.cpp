#include "main.h"
#include "init.h"

CLogModule        Log;
CSpiModule        Spi;
CBlueNRGModule    BlueNRG;
CI2CModule        I2C;
CSensorModule     Sensor;

int main(void)
{
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
   }
}

void CommandExecute(unsigned int u32CommandCode)
{
   Log.StrHexR("CMD ", u32CommandCode, 1);

   switch(u32CommandCode)
   {
   case 1:
      Sensor.Handle();
      break;
   }
}
