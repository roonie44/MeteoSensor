#include "main.h"

void CLogModule::Init(void)
{
   // Init TX
   s32IdxWrite = s32IdxSend = 0;

   // Init RX
   LL_DMA_DisableChannel(DMA1, DMA_CHANNEL_LOG_RX);
   LL_DMA_SetMemoryAddress(DMA1, DMA_CHANNEL_LOG_RX, (unsigned int)&U8InputBuffer[0]);
   LL_DMA_SetDataLength(DMA1, DMA_CHANNEL_LOG_RX, LOG_INPUT_BUFFER_SIZE);
   LL_DMA_EnableChannel(DMA1, DMA_CHANNEL_LOG_RX);
}

void CLogModule::HandleTx(void)
{
   int   s32DataToSend;

   if(s32IdxWrite == s32IdxSend)
      return;

   LL_DMA_DisableChannel(DMA1, DMA_CHANNEL_LOG_TX);

   // Data not sent yet
   s32DataToSend = LL_DMA_GetDataLength(DMA1, DMA_CHANNEL_LOG_TX);
   if(s32DataToSend != 0)
   {
      LL_DMA_EnableChannel(DMA1, DMA_CHANNEL_LOG_TX);
      return;
   }

   LL_DMA_SetMemoryAddress(DMA1, DMA_CHANNEL_LOG_TX, (unsigned int)&U8OutputBuffer[s32IdxSend]);

   if(s32IdxWrite > s32IdxSend)
   {
      s32DataToSend  = s32IdxWrite - s32IdxSend;
      s32IdxSend     = s32IdxWrite;
   }
   else // s32IdxWrite < s32IdxSend
   {
      s32DataToSend  = sizeof(U8OutputBuffer) - s32IdxSend;
      s32IdxSend     = 0;
   }

   LL_DMA_SetDataLength(DMA1, DMA_CHANNEL_LOG_TX, s32DataToSend);
   LL_DMA_EnableChannel(DMA1, DMA_CHANNEL_LOG_TX);
}

void CLogModule::NewLine(void)
{
   U8OutputBuffer[s32IdxWrite] = '\r';
   s32IdxWrite = (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
   if(LL_DMA_GetDataLength(DMA1, DMA_CHANNEL_LOG_TX) == 0)
      HandleTx();
}

void CLogModule::Str(const char* pString)
{
   while(*pString)
   {
      U8OutputBuffer[s32IdxWrite] = *pString;
      s32IdxWrite = (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
      pString++;
   }
   if(LL_DMA_GetDataLength(DMA1, DMA_CHANNEL_LOG_TX) == 0)
      HandleTx();
}

void CLogModule::Hex(unsigned int u32Number, unsigned int u32Size)
{
	TUINT Number;
	Number.uInt = u32Number;

	switch(u32Size)
	{
	default:
	U8OutputBuffer[s32IdxWrite] 	= ASCII(Number.uNibbleHHH);
	s32IdxWrite 						= (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
	U8OutputBuffer[s32IdxWrite] 	= ASCII(Number.uNibbleHHL);
	s32IdxWrite 						= (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
	case 3:
	U8OutputBuffer[s32IdxWrite] 	= ASCII(Number.uNibbleHLH);
	s32IdxWrite 						= (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
	U8OutputBuffer[s32IdxWrite] 	= ASCII(Number.uNibbleHLL);
	s32IdxWrite 						= (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
	case 2:
	U8OutputBuffer[s32IdxWrite] 	= ASCII(Number.uNibbleLHH);
	s32IdxWrite 						= (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
	U8OutputBuffer[s32IdxWrite] 	= ASCII(Number.uNibbleLHL);
	s32IdxWrite 						= (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
	case 1:
	U8OutputBuffer[s32IdxWrite] 	= ASCII(Number.uNibbleLLH);
	s32IdxWrite 						= (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
	U8OutputBuffer[s32IdxWrite] 	= ASCII(Number.uNibbleLLL);
	s32IdxWrite 						= (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
	}
   if(LL_DMA_GetDataLength(DMA1, DMA_CHANNEL_LOG_TX) == 0)
      HandleTx();
}

void CLogModule::StrHex(const char* pString, unsigned int u32Number, unsigned int u32Size)
{
   Str(pString);
   Hex(u32Number, u32Size);
}

void CLogModule::StrHexR(const char* pString, unsigned int u32Number, unsigned int u32Size)
{
   Str(pString);
   Hex(u32Number, u32Size);
   NewLine();
}

void CLogModule::Dec(unsigned int u32Number)
{
   unsigned char U8OutputDec[10];
   int s32Len = 0;

   do
   {
      U8OutputDec[s32Len] =  u32Number % 10;
      u32Number /= 10;
      s32Len++;
   }
   while(u32Number > 0);

   while(s32Len > 0)
   {
      s32Len--;
      U8OutputBuffer[s32IdxWrite] = ASCII(U8OutputDec[s32Len]);
      s32IdxWrite = (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
   }
   if(LL_DMA_GetDataLength(DMA1, DMA_CHANNEL_LOG_TX) == 0)
      HandleTx();
}

void CLogModule::StrDec(const char* pString, unsigned int u32Number)
{
   Str(pString);
   Dec(u32Number);
}

void CLogModule::StrDecR(const char* pString, unsigned int u32Number)
{
   Str(pString);
   Dec(u32Number);
   NewLine();
}

void CLogModule::Blob(unsigned char* U8Blob, unsigned int u32Length)
{
   TUCHAR*  pBlob = (TUCHAR*)U8Blob;

   while(u32Length--)
   {
      U8OutputBuffer[s32IdxWrite]   = ASCII(pBlob->uNibbleH);
      s32IdxWrite                   = (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
      U8OutputBuffer[s32IdxWrite]   = ASCII(pBlob->uNibbleL);
      s32IdxWrite                   = (s32IdxWrite + 1) % sizeof(U8OutputBuffer);
      pBlob++;
   }
}

void CLogModule::StrBlob(const char* pString, unsigned char* U8Blob, unsigned int u32Length)
{
   Str(pString);
   Blob(U8Blob, u32Length);
}

void CLogModule::StrBlobR(const char* pString, unsigned char* U8Blob, unsigned int u32Length)
{
   Str(pString);
   Blob(U8Blob, u32Length);
   NewLine();
}

void CLogModule::HandleRx(void)
{
   int s32DataReceived;

   LL_DMA_DisableChannel(DMA1, DMA_CHANNEL_LOG_RX);
   s32DataReceived = LOG_INPUT_BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, DMA_CHANNEL_LOG_RX);

   // Przetwarzanie
   StrBlobR("LOG RX ", U8InputBuffer, s32DataReceived);

   if(U8InputBuffer[0] == 'C' && U8InputBuffer[1] == 'M' && U8InputBuffer[2] == 'D')
   {
      CommandExecute(1);
   }


   LL_DMA_SetMemoryAddress(DMA1, DMA_CHANNEL_LOG_RX, (unsigned int)&U8InputBuffer[0]);
   LL_DMA_SetDataLength(DMA1, DMA_CHANNEL_LOG_RX, LOG_INPUT_BUFFER_SIZE);
   LL_DMA_EnableChannel(DMA1, DMA_CHANNEL_LOG_RX);
}
