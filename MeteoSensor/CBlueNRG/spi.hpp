#pragma once

class CSpiModule
{
public:
   int            ReadWrite      (unsigned char* U8Data, int s32Length);
   void           EnableCS       (void);
   void           DisableCS      (void);
};
