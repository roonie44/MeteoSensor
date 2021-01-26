#pragma once

#include "module.hpp"

class CSensorModule : public CModule
{
public:
   enum class eDataType
   {
      Temperature,
      Humidity,
      Pressure,
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
      bool  bPressure;
   } MeasurementRequest;
};
