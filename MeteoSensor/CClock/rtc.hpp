#pragma once

#include "module.hpp"

class CRtcModule : public CModule
{
public:
   void  SetPeriodicWakeUp (unsigned short u16Seconds);
   void  InterruptWakeUp   ();
};
