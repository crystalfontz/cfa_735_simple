/*
 * Description:
 *
 * Defines for the Pins and Ports used on the STM32F103
 *
 * License:
 *
 * Copyright 2012 Crystalfontz America, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#if defined USE_CFA_735_V0_9
  #define USB_DISCONNECT            GPIOC
  #define USB_DISCONNECT_CLOCK      RCC_APB2Periph_GPIOC
  #define USB_DISCONNECT_PIN        GPIO_Pin_13

  #define SDCARD_SPI                SPI2
  #define SDCARD_SPI_RCC_APB1       RCC_APB1Periph_SPI2
  #define SDCARD_SPI_Remap          0
  #define SDCARD_SPI_GPIO           GPIOB
  #define SDCARD_SPI_GPIO_RCC_APB2  RCC_APB2Periph_GPIOB
  #define SDCARD_SPI_MISO_Pin       GPIO_Pin_14
  #define SDCARD_SPI_SCK_Pin        GPIO_Pin_13
  #define SDCARD_SPI_MOSI_Pin       GPIO_Pin_15
  #define SDCARD_SPI_CS_GPIO        GPIOB
  #define SDCARD_SPI_CS_GPIO_RCC_APB2 RCC_APB2Periph_GPIOB
  #define SDCARD_SPI_CS_Pin         GPIO_Pin_12


  #define BKLT_ENA_PORT_CLOCKS (RCC_APB2Periph_GPIOC)
  #define KEY_BKLT_ENA_PORT   GPIOC
  #define KEY_BKLT_ENA_PIN    GPIO_Pin_14
  #define LCD_BKLT_ENA_PORT   GPIOC
  #define LCD_BKLT_ENA_PIN    GPIO_Pin_15

  #define BKLT_DAC_PORT_CLOCKS (RCC_APB2Periph_GPIOA)
  #define KEY_BKLT_DAC_PORT   GPIOA
  #define KEY_BKLT_DAC_PIN    GPIO_Pin_4
  #define LCD_BKLT_DAC_PORT   GPIOA
  #define LCD_BKLT_DAC_PIN    GPIO_Pin_5

  /* ST7529 Data Lines */
  #define ST7529_DATA_GPIO            GPIOC
  #define ST7529_DATA_GPIO_CLK        RCC_APB2Periph_GPIOC
  #define ST7529_DATA_PINS            (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)

  #define ST7529_A0               GPIO_Pin_8
  #define ST7529_A0_GPIO          GPIOC
  #define ST7529_A0_GPIO_CLK      RCC_APB2Periph_GPIOC

  #define ST7529_nWR              GPIO_Pin_7
  #define ST7529_nWR_GPIO         GPIOA
  #define ST7529_nWR_GPIO_CLK     RCC_APB2Periph_GPIOA

  #define ST7529_nRD              GPIO_Pin_6
  #define ST7529_nRD_GPIO         GPIOA
  #define ST7529_nRD_GPIO_CLK     RCC_APB2Periph_GPIOA

  #define ST7529_nCS              GPIO_Pin_9
  #define ST7529_nCS_GPIO         GPIOC
  #define ST7529_nCS_GPIO_CLK     RCC_APB2Periph_GPIOC

  #define ST7529_nRST             GPIO_Pin_2
  #define ST7529_nRST_GPIO        GPIOB
  #define ST7529_nRST_GPIO_CLK    RCC_APB2Periph_GPIOB


  /* Keypad Lines, shared with ST7529 data lines */
  #define KEYS_GPIO            GPIOC
  #define KEYS_GPIO_CLK        RCC_APB2Periph_GPIOC
  #define KEY_UP_PIN           GPIO_Pin_0
  #define KEY_DOWN_PIN         GPIO_Pin_1
  #define KEY_LEFT_PIN         GPIO_Pin_2
  #define KEY_RIGHT_PIN        GPIO_Pin_3
  #define KEY_ENTER_PIN        GPIO_Pin_4
  #define KEY_CANCEL_PIN       GPIO_Pin_5

  #define LED1G_GPIO		GPIOA
  #define LED1G_PIN		GPIO_Pin_0
  #define LED1G_GPIO_CLK	RCC_APB2Periph_GPIOA
  #define LED1R_GPIO		GPIOA
  #define LED1R_PIN		GPIO_Pin_1
  #define LED1R_GPIO_CLK	RCC_APB2Periph_GPIOA
  #define LED2G_GPIO		GPIOA
  #define LED2G_PIN		GPIO_Pin_2
  #define LED2G_GPIO_CLK	RCC_APB2Periph_GPIOA
  #define LED2R_GPIO		GPIOA
  #define LED2R_PIN		GPIO_Pin_3
  #define LED2R_GPIO_CLK	RCC_APB2Periph_GPIOA
  #define LED3G_GPIO		GPIOB
  #define LED3G_PIN		GPIO_Pin_0
  #define LED3G_GPIO_CLK	RCC_APB2Periph_GPIOB
  #define LED3R_GPIO		GPIOB
  #define LED3R_PIN		GPIO_Pin_1
  #define LED3R_GPIO_CLK	RCC_APB2Periph_GPIOB
  #define LED4G_GPIO		GPIOC
  #define LED4G_PIN		GPIO_Pin_12
  #define LED4G_GPIO_CLK	RCC_APB2Periph_GPIOC
  #define LED4R_GPIO		GPIOD
  #define LED4R_PIN		GPIO_Pin_2
  #define LED4R_GPIO_CLK	RCC_APB2Periph_GPIOD


  #define H1UART		USART3
  #define H1UART_CLK		RCC_APB1Periph_USART3
  #define H1UART_CLK_CMD	RCC_APB1PeriphClockCmd
  #define H1UART_IRQ		USART3_IRQn
  #define H1UART_HANDLER	USART3_IRQHandler
  #define H1UART_REMAP		GPIO_PartialRemap_USART3
  #define H1UART_GPIO_PORT	GPIOB
  #define H1UART_GPIO_PORT_CLK	RCC_APB2Periph_GPIOB
  #define H1UART_GPIO_TX	GPIO_Pin_10
  #define H1UART_GPIO_RX	GPIO_Pin_11

#endif

#endif /* __PLATFORM_CONFIG_H */

