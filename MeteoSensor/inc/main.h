#pragma once

#include <string.h>
#include "stm32l4xx.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_rtc.h"

#include "hardware_MeteoNode.h"
#include "types.h"

#include "../CLog/log.hpp"
#include "../CData/data.hpp"
#include "../CPower/power.hpp"
#include "../CClock/rtc.hpp"
#include "../CClock/clock.hpp"
#include "../CBlueNRG/spi.hpp"
#include "../CBlueNRG/bluenrg.hpp"
#include "../CSensor/i2c.hpp"
#include "../CSensor/BME280.hpp"
#include "../CSensor/sensor.hpp"

extern const char       DEVICE_NAME[32];
extern const char       BUILD_DATE[32];

extern CLogModule       Log;
extern CData            Data;
extern CPowerModule     Power;
extern CRTC             Rtc;
extern CSpiModule       Spi;
extern CBlueNRGModule   BlueNRG;
extern CI2C             I2C;
extern CBME280          BME280;
extern CSensorModule    Sensor;

int   main           (void);
void  CommandExecute (unsigned int u32CommandCode);
