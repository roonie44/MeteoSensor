#pragma once

#include "module.hpp"

enum class EventId
{
   None,
   DeviceIdle,
   PeriodicWakeUp,
   DataUpdateTemperature,
   DataUpdateHumidity,
   DataUpdatePressure,
};

class CEvents
{
   static CModule* Subscribers[10];
   static EventId  CurrentEventId;

public:
   static  void      Subscribe   (CModule *pSubscriber);
   static  void      Publish     (EventId EventId);
   static  EventId   GetEventId  ();
};
