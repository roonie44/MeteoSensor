#include "../inc/events.hpp"

#include "stm32l4xx.h"

CModule* CEvents::Subscribers[10]  = { nullptr, };
EventId  CEvents::CurrentEventId   = EventId::None;

void CEvents::Subscribe(CModule *pSubscriber)
{
   for (unsigned int Idx = 0; Idx < sizeof(Subscribers); Idx++)
   {
      if (Subscribers[Idx] == pSubscriber)
         break;
      if (Subscribers[Idx] == nullptr)
      {
         Subscribers[Idx] = pSubscriber;
         break;
      }
   }
}

void CEvents::Publish(EventId EventId)
{
   __disable_irq();
   CurrentEventId = EventId;
   for (unsigned int Idx = 0; Idx < sizeof(Subscribers); Idx++)
   {
      if (Subscribers[Idx] != nullptr)
      {
         Subscribers[Idx]->Event();
      }
      else break;
   }
   CurrentEventId = EventId::None;
   __enable_irq();
}

EventId CEvents::GetEventId()
{
   return CurrentEventId;
}
