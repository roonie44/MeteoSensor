#ifndef HARDWARE_H_
#define HARDWARE_H_

#define SYSCLK_FREQ		4000000

// Pattern
#define PIN__PIN              	         LL_GPIO_PIN_
#define PIN__PORT             	         GPIO

// Logs
#define PIN_LOG_USART_TX_PIN              LL_GPIO_PIN_2
#define PIN_LOG_USART_RX_PIN              LL_GPIO_PIN_3
#define PIN_LOG_USART_PORT                GPIOA
#define PIN_LOG_USART_AF                  LL_GPIO_AF_7
#define USART_LOG                         USART2
#define DMA_CHANNEL_LOG_TX                LL_DMA_CHANNEL_7
#define DMA_CHANNEL_LOG_RX                LL_DMA_CHANNEL_6
#define DMA_REQUEST_LOG                   LL_DMA_REQUEST_2

// BlueNRG-MS
#define PIN_BLUENRG_SPI_CLK_PIN           LL_GPIO_PIN_3
#define PIN_BLUENRG_SPI_CLK_PORT          GPIOB
#define PIN_BLUENRG_SPI_MISO_PIN          LL_GPIO_PIN_6
#define PIN_BLUENRG_SPI_MISO_PORT         GPIOA
#define PIN_BLUENRG_SPI_MOSI_PIN          LL_GPIO_PIN_7
#define PIN_BLUENRG_SPI_MOSI_PORT         GPIOA
#define PIN_BLUENRG_SPI_RESET_PIN         LL_GPIO_PIN_8
#define PIN_BLUENRG_SPI_RESET_PORT        GPIOA
#define PIN_BLUENRG_SPI_NCS_PIN           LL_GPIO_PIN_1
#define PIN_BLUENRG_SPI_NCS_PORT          GPIOA
#define PIN_BLUENRG_SPI_IRQ_PIN           LL_GPIO_PIN_0
#define PIN_BLUENRG_SPI_IRQ_PORT          GPIOA
#define PIN_BLUENRG_SPI_AF                LL_GPIO_AF_5
#define SPI_BLUENRG                       SPI1

// HTS221 - Temperature/humidity sensor
// LPS25HB - Pressure sensor
#define PIN_SENSOR_I2C_SDA_PIN            LL_GPIO_PIN_8
#define PIN_SENSOR_I2C_SDA_PORT           GPIOB
#define PIN_SENSOR_I2C_SDA_AF             LL_GPIO_AF_4
#define PIN_SENSOR_I2C_SCL_PIN            LL_GPIO_PIN_9
#define PIN_SENSOR_I2C_SCL_PORT           GPIOB
#define PIN_SENSOR_I2C_SCL_AF             LL_GPIO_AF_4
#define I2C_SENSOR                        I2C1
#define PIN_HTS221_I2C_DRDY_PIN           LL_GPIO_PIN_10
#define PIN_HTS221_I2C_DRDY_PORT          GPIOB
#define I2C_ADDRESS_HTS221                0xBE
#define PIN_LPS25HB_I2C_INT_PIN           LL_GPIO_PIN_4
#define PIN_LPS25HB_I2C_INT_PORT          GPIOB
#define I2C_ADDRESS_LPS25HB               0x00

// LED
#define PIN_LED_GREEN_PIN                 LL_GPIO_PIN_5
#define PIN_LED_GREEN_PORT                GPIOA

#endif /* HARDWARE_H_ */
