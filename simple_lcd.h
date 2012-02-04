/*
 * Description:
 *
 * Simple function header for interfacing with the LCD and rendering characters.
 *
 * The LCD is implemented as an 8-bit (1 byte per pixel) framebuffer.
 *
 * Render coordinates are where the upper left corner of the character will start.
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
#ifndef __SIMPLE_LCD_H__
#define __SIMPLE_LCD_H__

/* initialize the LCD controller */
void LCDInit();
/* clear the frame buffer */
void LCDClear();
/* enable / disable the LCD backlight */
void LCDBacklightOn(int onoff);
/* render a string at an arbitrary x,y location */
void RenderString(unsigned int x, unsigned int y, const char* string);
/* render a number as hexidecimal at an arbitrary x,y location */
void RenderHexNumber(unsigned int x, unsigned int y, uint32_t num);
/* render a number as decimal at an arbitrary x,y location */
void RenderNumber(unsigned int x, unsigned int y, uint32_t num);
/* Push the framebuffer to the LCD controller */
void PushBuffer(void);

#endif //__SIMPLE_LCD_H__
