#include "main.h"
#include "init.h"

CLogModule        Log;
CSpiModule        Spi;
CBlueNRGModule    BlueNRG;
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

   LL_GPIO_SetOutputPin(PIN_BLUENRG_SPI_RESET_PORT, PIN_BLUENRG_SPI_RESET_PIN);

   while(1)
   {
      if(BlueNRG.HandleRequest)
         BlueNRG.Handle();

      if(Sensor.HandleRequest)
         Sensor.Handle();
   }
}
