/*
 * Description:
 *
 * Bi-color LED functions and pin maps
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
#include "leds.h"
#include "platform_config.h"

#include "stm32f10x.h"

/* define a list of the LED mappings */

struct LED {
    GPIO_TypeDef* Port;
    uint16_t Pin;
    uint32_t Clock;
};

#define LED_COUNT 8

static const struct LED leds[LED_COUNT] = {
    {LED1G_GPIO, LED1G_PIN, LED1G_GPIO_CLK},
    {LED2G_GPIO, LED2G_PIN, LED2G_GPIO_CLK},
    {LED3G_GPIO, LED3G_PIN, LED3G_GPIO_CLK},
    {LED4G_GPIO, LED4G_PIN, LED4G_GPIO_CLK},
    {LED1R_GPIO, LED1R_PIN, LED1R_GPIO_CLK},
    {LED2R_GPIO, LED2R_PIN, LED2R_GPIO_CLK},
    {LED3R_GPIO, LED3R_PIN, LED3R_GPIO_CLK},
    {LED4R_GPIO, LED4R_PIN, LED4R_GPIO_CLK},
};

/* init each LED in the table */
void LEDsInit(void)
{
    int i;
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);

    for (i = 0; i < LED_COUNT; i++)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Pin = leds[i].Pin;

        RCC_APB2PeriphClockCmd(leds[i].Clock, ENABLE);
        GPIO_Init(leds[i].Port, &GPIO_InitStructure);
    }
}

/* walk the LED table, toggling the next LED every count calls */
void LEDsWalk(unsigned int walk_inc_count)
{
    static unsigned int i = 0;
    static uint8_t toggle = 0;

    if (i == walk_inc_count)
    {
        if (GPIO_ReadOutputDataBit(leds[toggle].Port, leds[toggle].Pin))
            GPIO_ResetBits(leds[toggle].Port, leds[toggle].Pin);
        else
            GPIO_SetBits(leds[toggle].Port, leds[toggle].Pin);

        /* walk next */
        ++toggle;
        if (toggle == LED_COUNT)
            toggle = 0;
        i = 0;
    }
    else
    {
        ++i;
    }
}

