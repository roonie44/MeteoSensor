#pragma once

class CPowerModule
{
   bool bSleepDeny;

public:
   void Handle          (void);

   void SleepDeny       (void);
   void SleepAllow      (void);

//private:
   void EnterStopMode   (void);
};
