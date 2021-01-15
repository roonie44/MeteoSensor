#pragma once

class CRTC
{
public:
         CRTC              ();
   void  SetWakeUpPeriod   (unsigned short u16Seconds);
   void  InterruptWakeUp   ();
};
