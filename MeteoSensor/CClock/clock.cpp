#include "main.h"

volatile unsigned int CClock::TICKS = 0;

void CClock::Init()
{
   LL_RCC_ClocksTypeDef RCC_Clocks;
   LL_RCC_GetSystemClocksFreq(&RCC_Clocks);
   SysTick_Config(RCC_Clocks.HCLK_Frequency / CLOCK_FREQUENCY);
}
void CClock::Deinit()
{
   SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
void CClock::Tick()
{
   TICKS++;
}
void CClock::Set()
{
   u32TicksStart = TICKS;
}
bool CClock::IsElapsed(unsigned int u32Mills)
{
   // Overturn
   if (u32TicksStart > __UINT32_MAX__ - u32Mills)
      if (u32TicksStart <= TICKS)
         return false;
   if (u32TicksStart + u32Mills <= TICKS)
      return true;
   return false;
}
void CClock::Wait(unsigned int u32Mills)
{
   Set();
   while (IsElapsed(u32Mills) == false);
}
