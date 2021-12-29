#pragma once

#include "module.hpp"

class CSensorModule : public CModule
{
public:
   enum class eDataType
   {
      Temperature,
      Humidity,
   };


         CSensorModule  ();
   void  Handle         ();
   void  Event          ();

   void  Init           ();
   void  DataRequest    (eDataType DataType);

private:
   struct
   {
      bool  bTemperature;
      bool  bHumidity;
   } MeasurementRequest;
};
