#include "main.h"
#include "init.h"

void CPowerModule::Handle(void)
{
   if(bSleepDeny == false)
   {
      EnterStopMode();
   }
}

void CPowerModule::SleepDeny(void)
{
   bSleepDeny = true;
}

void CPowerModule::SleepAllow(void)
{
   bSleepDeny = false;
}

void CPowerModule::EnterStopMode(void)
{
   Log.Str("\r");
   Log.Wait();

   CClock::Deinit();

   __disable_irq();

   if (bSleepDeny == false)
   {
      Init_Stop();

      // Disable internal wakeup line
      PWR->CR3 &= ~PWR_CR3_EIWF;

      // Clear all wake up Flags
      PWR->SCR |= PWR_SCR_CWUF;

      // Select wakeup pin polarity (0: rising, 1: falling)
      PWR->CR4 &= ~PWR_CR4_WP1;
      // Enable wakeup pin WKUP1 (PA0)
      PWR->CR3 |= LL_PWR_WAKEUP_PIN1;

      // Set STOP2 mode when CPU enters deepsleep
      LL_PWR_SetPowerMode(LL_PWR_MODE_STOP2);

      // Set SLEEPDEEP bit of Cortex System Control Register
      LL_LPM_EnableDeepSleep();

      /* Request Wait For Interrupt */
      __WFI();

      LL_LPM_EnableSleep();

      Init_Wakeup();
   }

   __enable_irq();

   CClock::Init();
}
