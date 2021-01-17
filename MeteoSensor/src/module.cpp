#include "../inc/module.hpp"
#include "../inc/events.hpp"

CModule* CModule::pHead = nullptr;

void CModule::HandleRequest(CModule *pRequester)
{
   if (pHead == nullptr)
      pHead = pRequester;
   else for (CModule *pModule = pHead; pModule != pRequester; pModule = pModule->pNext)
   {
      if (pModule->pNext == nullptr)
      {
         pModule->pNext = pRequester;
         break;
      }
   }
}

bool CModule::IsIdle()
{
   return (pHead == nullptr);
}

void CModule::NextHandle()
{
   if (pHead != nullptr)
   {
      CModule *pModuleToHandle = pHead;
      pHead = pHead->pNext;
      pModuleToHandle->pNext = nullptr;
      pModuleToHandle->Handle();
   }
   else
      CEvents::Publish(EventId::DeviceIdle);
}
