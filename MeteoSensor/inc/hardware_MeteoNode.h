#ifndef HARDWARE_H_
#define HARDWARE_H_

// Pattern
#define PIN__PIN              	         LL_GPIO_PIN_
#define PIN__PORT             	         GPIO

// Logs
#define PIN_LOG_USART_TX_PIN              LL_GPIO_PIN_10
#define PIN_LOG_USART_RX_PIN              LL_GPIO_PIN_11
#define PIN_LOG_USART_PORT                GPIOC
#define PIN_LOG_USART_AF                  LL_GPIO_AF_7
#define USART_LOG                         USART3
#define DMA_CHANNEL_LOG_TX                LL_DMA_CHANNEL_2
#define DMA_CHANNEL_LOG_RX                LL_DMA_CHANNEL_3
#define DMA_REQUEST_LOG                   LL_DMA_REQUEST_2
#define DMA_LOG                           DMA1

// BlueNRG-MS
#define PIN_BLUENRG_SPI_CLK_PIN           LL_GPIO_PIN_5
#define PIN_BLUENRG_SPI_CLK_PORT          GPIOA
#define PIN_BLUENRG_SPI_MISO_PIN          LL_GPIO_PIN_6
#define PIN_BLUENRG_SPI_MISO_PORT         GPIOA
#define PIN_BLUENRG_SPI_MOSI_PIN          LL_GPIO_PIN_7
#define PIN_BLUENRG_SPI_MOSI_PORT         GPIOA
#define PIN_BLUENRG_SPI_RESET_PIN         LL_GPIO_PIN_5
#define PIN_BLUENRG_SPI_RESET_PORT        GPIOC
#define PIN_BLUENRG_SPI_NCS_PIN           LL_GPIO_PIN_4
#define PIN_BLUENRG_SPI_NCS_PORT          GPIOC
#define PIN_BLUENRG_SPI_IRQ_PIN           LL_GPIO_PIN_0
#define PIN_BLUENRG_SPI_IRQ_PORT          GPIOA
#define PIN_BLUENRG_SPI_AF                LL_GPIO_AF_5
#define SPI_BLUENRG                       SPI1

// Sensor BME280
#define PIN_SENSOR_I2C_SCL_PIN            LL_GPIO_PIN_10
#define PIN_SENSOR_I2C_SCL_PORT           GPIOB
#define PIN_SENSOR_I2C_SCL_AF             LL_GPIO_AF_4
#define PIN_SENSOR_I2C_SDA_PIN            LL_GPIO_PIN_11
#define PIN_SENSOR_I2C_SDA_PORT           GPIOB
#define PIN_SENSOR_I2C_SDA_AF             LL_GPIO_AF_4
#define PIN_SENSOR_I2C_PWR_PIN            LL_GPIO_PIN_12
#define PIN_SENSOR_I2C_PWR_PORT           GPIOB
#define I2C_SENSOR                        I2C2

// LED
#define PIN_LED_GREEN_PIN                 LL_GPIO_PIN_1
#define PIN_LED_GREEN_PORT                GPIOB

#endif /* HARDWARE_H_ */
