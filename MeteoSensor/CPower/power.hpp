#ifndef POWER_HPP_
#define POWER_HPP_

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

#endif /* POWER_HPP_ */
