#pragma once

class CHDC1080
{
public:
                  CHDC1080                (CI2C *pI2C) { this->pI2C = pI2C; }
   void           Init                    ();
   Status         GetTemperature          (signed int& s32Temperature);
   Status         GetHumidity             (unsigned int& u32Humidity);

   Status         PowerOn                 ();
   void           PowerOff                ();

private:
   static const unsigned char I2C_ADDR                   = 0b1000000;
   static const unsigned char REG_ADDR_TEMP              = 0x00;
   static const unsigned char REG_ADDR_HUMI              = 0x01;
   static const unsigned char REG_ADDR_CONFIG            = 0x02;
   static const unsigned char REG_ADDR_ID_MANUFACTURER   = 0xFE;
   static const unsigned char REG_ADDR_ID_DEVICE         = 0xFF;
   static const unsigned char TIMEOUT_POWER_ON           = 15; // ms
   static const unsigned char TIMEOUT_CONVERTION         = 10; // ms
   CI2C                       *pI2C;
};
