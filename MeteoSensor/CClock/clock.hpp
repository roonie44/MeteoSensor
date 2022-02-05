#pragma once

class CClock
{
   static const    unsigned int CLOCK_FREQUENCY = 1000; // Hz
   static volatile unsigned int TICKS;

                   unsigned int u32TicksStart;

public:
               CClock            () { this->Set(); }
   static void Init              ();
   static void Deinit            ();
   static void Tick              ();
          void Set               ();
          bool IsElapsed         (unsigned int u32Mills);
          void Wait              (unsigned int u32Mills);
};
