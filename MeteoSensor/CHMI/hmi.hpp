#pragma once

#include "module.hpp"
#include "events.hpp"

class CHmiModule  : public CModule
{
public:
         CHmiModule     ();
   void  Handle         ();
   void  Event          ();

private:
   void  LedBlink       ();
};
