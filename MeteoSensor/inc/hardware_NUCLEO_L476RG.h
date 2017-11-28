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
#define PIN_BLUETOOTH_SPI_CLK_PIN         LL_GPIO_PIN_3
#define PIN_BLUETOOTH_SPI_CLK_PORT        GPIOB
#define PIN_BLUETOOTH_SPI_MISO_PIN        LL_GPIO_PIN_6
#define PIN_BLUETOOTH_SPI_MISO_PORT       GPIOA
#define PIN_BLUETOOTH_SPI_MOSI_PIN        LL_GPIO_PIN_7
#define PIN_BLUETOOTH_SPI_MOSI_PORT       GPIOA
#define PIN_BLUETOOTH_SPI_RESET_PIN       LL_GPIO_PIN_8
#define PIN_BLUETOOTH_SPI_RESET_PORT      GPIOA
#define PIN_BLUETOOTH_SPI_NCS_PIN         LL_GPIO_PIN_1
#define PIN_BLUETOOTH_SPI_NCS_PORT        GPIOA
#define PIN_BLUETOOTH_SPI_IRQ_PIN         LL_GPIO_PIN_0
#define PIN_BLUETOOTH_SPI_IRQ_PORT        GPIOA
#define PIN_BLUETOOTH_SPI_AF              LL_GPIO_AF_5
#define SPI_BLUETOOTH                     SPI1

// LED
#define PIN_LED_GREEN_PIN                 LL_GPIO_PIN_5
#define PIN_LED_GREEN_PORT                GPIOA

#endif /* HARDWARE_H_ */
