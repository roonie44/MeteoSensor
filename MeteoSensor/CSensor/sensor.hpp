#pragma once

#include "module.hpp"

class CSensorModule : public CModule
{
public:
   enum eDataType
   {
      NONE,
      TEMPERATURE,
      HUMIDITY,
      PRESSURE,
   };

         CSensorModule  ();
   void  Handle         ();
   void  Event          ();

   void  Init           ();
   void  DataRequest    (eDataType DataType, unsigned int u32CallbackId);

private:
   bool           MeasurementRequest;
   eDataType      RequestDataType;
   unsigned int   u32CallbackId;
};
