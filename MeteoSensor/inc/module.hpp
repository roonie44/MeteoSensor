#pragma once

class CModule
{
   static CModule *pHead;
          CModule *pNext;

protected:
   static  void   HandleRequest  (CModule *pRequester);
   static  bool   IsIdle         ();

public:
   static  void   NextHandle     ();

   virtual void   Handle         ()                      { }
   virtual void   Callback       (CModule *pCaller)      { }
   virtual void   Event          ()                      { }
};
