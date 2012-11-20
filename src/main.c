/*
 * Description:
 *
 *
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
/*
 * Application's main entry point.
 */

#include "stm32f10x.h"
#include "platform_config.h"
#include "systick.h"
#include "lcd.h"
#include "keys.h"
#include "leds.h"
#include "uart.h"
#include "usb_vcom.h"
#include "ring_buffer.h"
#include "08x08fnt.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "process_cmd.h"

/* forward declarations */
void SetupInterruptVectors(void);
void SetupSysTick(void);

void ShowKeys(uint16_t key_state, unsigned int y_start);
void ShowH1UARTData(unsigned int y);
void SendKeysToH1UART(uint16_t key_state);
void ShowUSBData(unsigned int y);
void SendKeysToUSB(uint16_t key_state);
void WalkLEDs(unsigned int walk_inc_count);

void main_init(void)
{
    /* clocks are set up on entry by the stm32 startup code */

    /* Proc setup */
    SetupInterruptVectors();
    SetupSysTick();

    /* Init the LCD */
    LCDInit();
    LCDBacklightOn(1);
    PushBuffer();

    /* light the keys */
    KeyBacklightOn(0);

    /* init the LED pins */
    LEDsInit();

    /* init and enable the serial UART for 115200 8N1*/
    UARTinit();
    UARTenable(115200, UARTParity_No);

    /* Describe the USB device and init it */
    USB_VCOMSetVendorString("Crystalfontz");
    USB_VCOMSetProductString("User CFA-735");
    USB_VCOMSetSerialNumberString("1001");

    USB_VCOMinit();

    /* render a banner */
    extern unsigned short start_screen[];
    //RenderScreen((uint16_t*)start_screen);

    PushBuffer();
}

static uint8_t rx_buffer[1024];
static unsigned int rx_buffer_size;
static unsigned int led_toggle_start;
extern uint32_t stat_uart_irq;
extern uint32_t stat_uart_tx;
extern uint32_t stat_uart_rx;
/* main loop */
int main(void)
{
	unsigned int size;

	main_init();
	rx_buffer_size = 0;

	while (1)
	{
		size = sizeof(rx_buffer);
		size = USB_VCOMread(size-rx_buffer_size, &rx_buffer[rx_buffer_size]);
		if (size > 0)
		{
			rx_buffer_size += size;
			process_command(rx_buffer, &rx_buffer_size);
		}
		PushBuffer();
	}
}

/* display the data received on the serial port and scroll it as it comes in */
void ShowH1UARTData(unsigned int y)
{
    static char characters[21] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', '\0'};
    char old_chars[20];
    char current_chars[20];
    unsigned int chars_read;

    int x = 10;
    RenderString(x, y, "UART: ");
    x += 8 * 6;
    chars_read = UARTread(20, current_chars);
    if (chars_read > 0)
    {
        memcpy(old_chars, characters, 20);
        memcpy(characters, old_chars + chars_read, 20 - chars_read);
        memcpy(characters + 20 - chars_read, current_chars, chars_read);
    }
    RenderString(x, y, characters);
}

/* send the names of the keys pressed over the serial port */
void SendKeysToH1UART(uint16_t key_state)
{
    if (key_state&KEY_UP_PIN)
        UARTwrite(3, "UP ");
    if (key_state&KEY_DOWN_PIN)
        UARTwrite(5, "DOWN ");
    if (key_state&KEY_LEFT_PIN)
        UARTwrite(5, "LEFT ");
    if (key_state&KEY_RIGHT_PIN)
        UARTwrite(6, "RIGHT ");
    if (key_state&KEY_ENTER_PIN)
        UARTwrite(6, "ENTER ");
    if (key_state&KEY_CANCEL_PIN)
        UARTwrite(7, "CANCEL ");
}

/* display the data received on the USB port and scroll it as it comes in */
void ShowUSBData(unsigned int y)
{
    static char characters[21] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', '\0'};
    char old_chars[20];
    char current_chars[20];
    unsigned int chars_read;

    int x = 10;
    RenderString(x, y, " USB: ");
    x += 8 * 6;
    chars_read = USB_VCOMread(20, current_chars);
    if (chars_read > 0)
    {
        memcpy(old_chars, characters, 20);
        memcpy(characters, old_chars + chars_read, 20 - chars_read);
        memcpy(characters + 20 - chars_read, current_chars, chars_read);
    }
    RenderString(x, y, characters);
}

/* send the names of the keys pressed over the USB port */
void SendKeysToUSB(uint16_t key_state)
{
    if (key_state&KEY_UP_PIN)
        USB_VCOMwrite(3, "UP ");
    if (key_state&KEY_DOWN_PIN)
        USB_VCOMwrite(5, "DOWN ");
    if (key_state&KEY_LEFT_PIN)
        USB_VCOMwrite(5, "LEFT ");
    if (key_state&KEY_RIGHT_PIN)
        USB_VCOMwrite(6, "RIGHT ");
    if (key_state&KEY_ENTER_PIN)
        USB_VCOMwrite(6, "ENTER ");
    if (key_state&KEY_CANCEL_PIN)
        USB_VCOMwrite(7, "CANCEL ");
}

/* Display the keys pressed on the LCD */
void ShowKeys(uint16_t key_state, unsigned int y_start)
{
    int x = 10;
    RenderString(x, y_start, "Keys: ");
    x += 8 * 6;
    RenderString(x, y_start, (key_state&KEY_UP_PIN)?"UP ":"   ");
    x += 8 * 3;
    RenderString(x, y_start, (key_state&KEY_DOWN_PIN)?"DOWN ":"     ");
    x += 8 * 5;
    RenderString(x, y_start, (key_state&KEY_LEFT_PIN)?"LEFT ":"     ");
    x += 8 * 5;
    RenderString(x, y_start, (key_state&KEY_RIGHT_PIN)?"RIGHT ":"      ");
    x = 60;
    RenderString(x, y_start+10, (key_state&KEY_ENTER_PIN)?"ENTER ":"      ");
    x += 8 * 6;
    RenderString(x, y_start+10, (key_state&KEY_CANCEL_PIN)?"CANCEL ":"      ");
}

/* set the location of the interrupt vector table as in flash and at offset 0 */
void SetupInterruptVectors(void)
{
    // Set the Vector Table base address
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

    /* Enable the Interrupts */
    __enable_irq();
}

/* Set up a system tick interrupt to count mSecs */
void SetupSysTick(void)
{
    RCC_ClocksTypeDef RCC_ClockFreq;

    // This function fills the RCC_ClockFreq structure with the current frequencies of
    // different on chip clocks (for debug purpose)
    RCC_GetClocksFreq(&RCC_ClockFreq);

    /* Setup a millisecond systick timer */
    mSecSysTickInit(RCC_ClockFreq.SYSCLK_Frequency);
}

/* provide a simple usleep function so a stndard library is not required*/
int usleep(useconds_t usec)
{
    unsigned int start = getSysTick_mSecs();
    while (getSysTick_mSecs() - start < usec/1000) ;
    return 0;
}

#ifdef  USE_FULL_ASSERT
/* if asserts are enabled this code will cause the code to break in the debugger */
void assert_failed(uint8_t* file, uint32_t line)
{
    asm volatile(
        "BKPT 0\n"
        );
    while(1);
}
#endif

/* simple memset implementation so a standard library is not required */
void *memset(void *s, int c, unsigned int n)
{
    char* t = (char*)s;
    while(n--)
        *(t++) = (char)c;
    return s;
}

/* simple memcpy implementation so a standard library is not required */
void *memcpy(void *d, const void *s, size_t n)
{
    while(n--)
        *((char*)d++) = *((char*)s++);
    return d;
}

/* simple memmove implementation so a standard library is not required */
void *memmove(void *d, const void *s, size_t n)
{
    while(n--)
        *((char*)d++) = *((char*)s++);
    return d;
}
