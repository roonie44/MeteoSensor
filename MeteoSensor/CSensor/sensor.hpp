#pragma once

#include "BME280.hpp"

class CSensorModule
{
public:
   enum eDataType
   {
      NONE,
      TEMPERATURE,
      HUMIDITY,
      PRESSURE,
   };

   bool  HandleRequest;

   void Init         ();
   void Handle       ();
   void DataRequest  (eDataType DataType, unsigned int u32CallbackId);

private:
   eDataType      RequestDataType;
   unsigned int   u32CallbackId;
};
