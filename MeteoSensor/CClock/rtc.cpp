#include "main.h"

CRTC::CRTC()
{

}

void CRTC::SetWakeUpPeriod(unsigned short u16Seconds)
{
   LL_RTC_DisableWriteProtection(RTC);
   LL_RTC_WAKEUP_Disable(RTC);
   if (u16Seconds > 0)
   {
      while(LL_RTC_IsActiveFlag_WUTW(RTC) == 0);
      LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_CKSPRE);
      LL_RTC_WAKEUP_SetAutoReload(RTC, u16Seconds);
      LL_RTC_EnableIT_WUT(RTC);
      LL_RTC_WAKEUP_Enable(RTC);
   }
   LL_RTC_EnableWriteProtection(RTC);
}

void CRTC::InterruptWakeUp()
{
   Sensor.DataRequest(CSensorModule::TEMPERATURE, 0);
   Sensor.DataRequest(CSensorModule::HUMIDITY, 0);
   Sensor.DataRequest(CSensorModule::PRESSURE, 0);
}
