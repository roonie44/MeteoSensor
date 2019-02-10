#ifndef MAIN_HPP_
#define MAIN_HPP_

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

#include "hardware_MeteoNode.h"
#include "types.h"

#include "../CLog/log.hpp"
#include "../CPower/power.hpp"
#include "../CBlueNRG/spi.hpp"
#include "../CBlueNRG/bluenrg.hpp"
#include "../CSensor/i2c.hpp"
#include "../CSensor/sensor.hpp"


extern CLogModule       Log;
extern CPowerModule     Power;
extern CSpiModule       Spi;
extern CBlueNRGModule   BlueNRG;
extern CI2CModule       I2C;
extern CSensorModule    Sensor;

int   main           (void);
void  CommandExecute (unsigned int u32CommandCode);
#endif // MAIN_HPP_
