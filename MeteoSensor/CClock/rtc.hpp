#pragma once

#include "module.hpp"

class CRTC : public CModule
{
public:
   void  SetPeriodicWakeUp (unsigned short u16Seconds);
   void  InterruptWakeUp   ();
};
