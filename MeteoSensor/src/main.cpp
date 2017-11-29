#include "main.h"
#include "init.h"

CLogModule       Log;

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

   while(1)
   {

   }
}
