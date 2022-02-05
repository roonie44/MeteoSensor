#include "main.h"

CHmiModule::CHmiModule()
{
   CEvents::Subscribe(this);
}

void CHmiModule::Handle()
{
   LedBlink();
}

void CHmiModule::Event()
{
   switch(CEvents::GetEventId())
   {
      case EventId::BleClientConnected:
      case EventId::BleClientDisconnected:
         CModule::HandleRequest(this);
         break;

      default:
         return;
   }
}

void CHmiModule::LedBlink()
{
   LL_GPIO_ResetOutputPin(PIN_LED_GREEN_PORT, PIN_LED_GREEN_PIN);
   CClock Clock;
   Clock.Wait(Data.GetLedBlinkTime());
   LL_GPIO_SetOutputPin(PIN_LED_GREEN_PORT, PIN_LED_GREEN_PIN);
}
