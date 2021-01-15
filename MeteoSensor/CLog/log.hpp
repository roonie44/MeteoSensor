#pragma once

#define LOG_OUTPUT_BUFFER_SIZE   8192
#define LOG_INPUT_BUFFER_SIZE   256

class CLogModule
{
   unsigned char  U8OutputBuffer [LOG_OUTPUT_BUFFER_SIZE];
   unsigned char  U8InputBuffer  [LOG_INPUT_BUFFER_SIZE];
   int            s32IdxWrite;
   int            s32IdxSend;

public:
   void  Init     (void);
   void  HandleTx (void);
   void  HandleRx (void);
   void  Wait     (void);

   void  NewLine  (void);
   void  Str      (const char* pString);

   void  Hex      (unsigned int u32Number, unsigned int u32Size = 4);
   void  StrHex   (const char* pString, unsigned int u32Number, unsigned int u32Size = 4);
   void  StrHexR  (const char* pString, unsigned int u32Number, unsigned int u32Size = 4);

   void  Dec      (unsigned int u32Number);
   void  StrDec   (const char* pString, unsigned int u32Number);
   void  StrDecR  (const char* pString, unsigned int u32Number);

   void  Blob     (unsigned char* U8Blob, unsigned int u32Length);
   void  StrBlob  (const char* pString, unsigned char* U8Blob, unsigned int u32Length);
   void  StrBlobR (const char* pString, unsigned char* U8Blob, unsigned int u32Length);
};
