/*
 * Description:
 *
 * Simple functions for interfacing with the LCD and rendering characters.
 *
 * The LCD is implemented as an 8-bit (1 byte per pixel) framebuffer.
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
#include "st7529_core.h"
#include "glassLayout.h"
#include "platform_config.h"
#include "lcd.h"
#include "stm32f10x.h"
#include <string.h>
#include "08x08fnt.h"

/* table of characters used to represent hex */
static const char hex_map[16] = {'0','1','2','3','4','5','6','7','8', '9','a','b','c','d','e','f'};

/* the framebuffer memory */
static uint8_t buffer[LCD_BUFFER_BYTE_CNT];


/* itialize the controller and display */
void LCDInit(void)
{
    ST7529_init();
    ST7529_writeContrast(LCD_CONTRAST_OPT);
    LCDClear();
}

/* clear the frame buffer */
void LCDClear(void)
{
    memset(buffer, WHITE, sizeof(buffer));
}

/* push the frame buffer to the controller */
void PushBuffer(void)
{
    /* as this bus is shared with the keys, we take it every push*/
    ST7529_busInit();
    /* push the buffer */
    ST7529_bufferToLCD(buffer);
}

/* enable disable the backlight */
void LCDBacklightOn(int onoff)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BKLT_ENA_PORT_CLOCKS, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LCD_BKLT_ENA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_BKLT_ENA_PORT, &GPIO_InitStructure);

    GPIO_WriteBit(LCD_BKLT_ENA_PORT, LCD_BKLT_ENA_PIN, onoff?Bit_SET:Bit_RESET);
}

/* convert an integer to a hex string and render it */
void RenderHexNumber(unsigned int x, unsigned int y, uint32_t num)
{
    unsigned int i;
    char s[9];
    s[8] = '\0';
    for (i = 0; i < 8; i++) {
        s[i] = hex_map[(num << (i*4)) >> 28];
    }
    RenderString(x, y, s);
}

/* convert an integer to a decimal string and render it */
void RenderNumber(unsigned int x, unsigned int y, uint32_t num)
{
    unsigned int i = 10;
    char s[11];
    s[10] = '\0';
    do
    {
        unsigned int a = num%10;
        --i;
        s[i] = hex_map[a];
        num /= 10;
    }
    while (i && num);
    RenderString(x, y, &s[i]);
}

/* render a c string at x,y */
void RenderString(unsigned int x, unsigned int y, const char* string)
{
    while (*string != '\0')
    {
        char c = *string;
        if (c < FONT_08X08_BASE || c > 148)
            c = ' ';

        Render( Font_08x08[(unsigned int)c - FONT_08X08_BASE], x, y);

        x+=8;
        ++string;
    }
}

/* render a c string at x,y */
void RenderString90(unsigned int x, unsigned int y, const char* string)
{
    while (*string != '\0')
    {
        char c = *string;
        if (c < FONT_08X08_BASE || c > 148)
            c = ' ';

        Render90( Font_Cyrillic_08x08[32], x, y);
        Render90( Font_Cyrillic_08x08[(unsigned int)c], x, y);

        x+=8;
        ++string;
    }
}

/* render an 8x8 bitmap (a character in 1bpp) at x,y */
void RenderScreen(const uint16_t *bitmap)
{
    uint8_t* fb = buffer;
    int bit;
    int pixels = 0;

    memset(buffer, 0, sizeof(buffer));


    while (fb < &(buffer[LCD_BUFFER_BYTE_CNT]))
    {
		bit = 0;
		while (bit < 16)
		{
            uint8_t val;
            val = (*bitmap >> bit);
            val &= 0x01;
            *fb = val ? 0xff:0x00;
            fb++;
            bit++;
    		pixels++;
            if (pixels >= 244)
            {
            	pixels = 0;
            	bit = 0;
            	bitmap++;
            }

		}
		bitmap++;
    }
}

/* negate 8x8 symbol at x,y */
void RenderNegate(unsigned int x, unsigned int y)
{
    uint8_t* fb = buffer + (y * LCD_USABLE_PIXELS_PER_ROW + x);

    unsigned int top, bottom;
    unsigned int left, right;

    top = 8;
    bottom = 0;

    do
    {
        --top;
        left = 0;
        right = 8;

        do
        {
            *fb = ~(*fb);
            fb++;

            ++left;
        } while (left != right);
        fb += LCD_USABLE_PIXELS_PER_ROW - 8;

    } while (top != bottom);
}

/* render an 8x8 bitmap (a character in 1bpp) at x,y */
void Render(const uint8_t bitmap[8], unsigned int x, unsigned int y)
{
    uint8_t* fb = buffer + (y * LCD_USABLE_PIXELS_PER_ROW + x);

    unsigned int top, bottom;
    unsigned int left, right;

    top = 8;
    bottom = 0;

    do
    {
        --top;
        left = 0;
        right = 8;

        do
        {
            uint8_t val;
            val = (bitmap[left]<<top);
            val &= 0x80;

            if (fb >= &(buffer[LCD_BUFFER_BYTE_CNT]))
                return;
            *fb = val?0xff:0x00;
            fb++;

            ++left;
        } while (left != right);
        fb += LCD_USABLE_PIXELS_PER_ROW - 8;

    } while (top != bottom);
}

/* render an 8x8 bitmap (a character in 1bpp) at x,y turned by 90 degrees */
void Render90(const uint8_t bitmap[8], unsigned int x, unsigned int y)
{
    uint8_t* fb = buffer + (y * LCD_USABLE_PIXELS_PER_ROW + x);

    unsigned int top, bottom;
    unsigned int left, right;


    left = 0;
    right = 8;
    do
    {

        top = 0;
        bottom = 8;
        do
        {
            uint8_t val;
            val = (bitmap[left]<<top);
            val &= 0x80;

            if (fb >= &(buffer[LCD_BUFFER_BYTE_CNT]))
                return;
            *fb = val?0xff:0x00;
            fb++;
            top++;

        } while (top != bottom);
        fb += LCD_USABLE_PIXELS_PER_ROW - 8;
        ++left;

    } while (left != right);
}
