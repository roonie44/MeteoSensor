#pragma once

#include "module.hpp"
#include "events.hpp"

class CPowerModule  : public CModule
{
public:
         CPowerModule   ();
   void  Handle         ();
   void  Event          ();

private:
   void EnterStopMode   (void);
};
