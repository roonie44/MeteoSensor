#include "LPS22HB.hpp"

void CLPS22HB::Init(void)
{

}

void CLPS22HB::CheckState(void)
{
//   unsigned char     U8RawData[2];
//
//   Log.Str("SEN LPS22HB Power UP\r");
//
//   I2C.Write(I2C_ADDRESS_LPS22HB, REG_ADDR_LPS22HB_WHO_AM_I);
//   I2C.Read(I2C_ADDRESS_LPS22HB, &U8RawData[0], 1);
//
//   Log.Str("SEN LPS22HB Status: ");
//   if(U8RawData[0] == REG_VAL_LPS22HB_WHO_AM_I)
//      Log.Str("OK\r");
//   else
//      Log.Str("ERR\r");
}

void CLPS22HB::NewMeasurement(void)
{
//   unsigned char U8RawData[3];
//
//   Log.Str("SEN LPS22HB Measurement request\r");
//
//   // Flush pending readouts
//   I2C.Write(I2C_ADDRESS_LPS22HB, REG_ADDR_LPS22HB_PRESS_OUT_XL);
//   I2C.Read(I2C_ADDRESS_LPS22HB, &U8RawData[0], sizeof(U8RawData));
//
//   // One-shot request with auto increment of register address
//   U8RawData[0] = REG_ADDR_LPS22HB_CTRL2;
//   U8RawData[1] = 0x11;
//   I2C.Write(I2C_ADDRESS_LPS22HB, U8RawData, 2);
//
//   do
//   {
//      // Read status
//      I2C.Write(I2C_ADDRESS_LPS22HB, REG_ADDR_LPS22HB_STATUS);
//      I2C.Read(I2C_ADDRESS_LPS22HB, &U8RawData[0], 1);
//   }
//   while((U8RawData[0] & 0x03) != 0x03);
}

void CLPS22HB::ReadPressure(void)
{
//   unsigned char  U8RawData[3];
//   unsigned int   u32Pressure = 0;
//
//   I2C.Write(I2C_ADDRESS_LPS22HB, REG_ADDR_LPS22HB_PRESS_OUT_XL);
//   I2C.Read(I2C_ADDRESS_LPS22HB, &U8RawData[0], sizeof(U8RawData));
//
//   // Pressure in 1/4096 [hPa]
//   u32Pressure = (unsigned int)U8RawData[0] | (unsigned int)U8RawData[1] << 8 | (unsigned int)U8RawData[2] << 16;
//
//   Log.StrDecR("SEN Pressure ", u32Pressure / 4096);
//
//   // Pressure in 0.1 [Pa] = 1/4096 [hPa] * 1000 / 4096
//   u32Pressure = (u32Pressure * 250) / 1024;
//
//   if(u32CallbackId != 0)
//      BlueNRG.Callback(u32CallbackId, &u32Pressure);
}
