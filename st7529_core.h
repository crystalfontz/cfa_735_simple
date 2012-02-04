/*
 * Description:
 *
 * Core functions for communicating with the ST7529 LCD controller
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
#ifndef __ST7529_CORE_H__
#define __ST7529_CORE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum { WHITE = 0, BLACK = 0b11111100 } gray_t;

/* contrast defines */
#define LCD_CONTRAST_MIN	130
#define LCD_CONTRAST_OPT	252
#define LCD_CONTRAST_INIT	LCD_CONTRAST_OPT
#define LCD_CONTRAST_MAX	350
#define LCD_CONTRAST_LO_COUNT	(LCD_CONTRAST_OPT - LCD_CONTRAST_MIN)
#define LCD_CONTRAST_HI_COUNT	(LCD_CONTRAST_MAX - LCD_CONTRAST_OPT)

/*
 * Core Functions
 */

/* init the pins on the LCD data bus for output */
void ST7529_busInit(void);
/* init the LCD controller */
void ST7529_init(void);
/* save the contrast adjustment so the contrast is set to LCD_CONTRAST_OPT it is optimal */
void ST7529_saveContrastAdjToEEPROM(void);
/* increase the contrast */
void ST7529_volumeUpContrast(void);
/* decrease the contrast */
void ST7529_volumeDownContrast(void);
/* write a contrast value to the controller */
void ST7529_writeContrast(uint16_t contrast);
/* write a buffer to the LCD controller */
void ST7529_bufferToLCD(uint8_t * bufferMem);

#ifdef __cplusplus
}
#endif
#endif //__ST7529_CORE_H__
