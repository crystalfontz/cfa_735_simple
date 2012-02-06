/*
 * Description:
 *
 * Functions to read the pins associated with the keys and enable the key backlight
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
#include "keys.h"
#include "stm32f10x.h"

/* read the keys' state on the key bus and return the bits on that bus */
uint16_t ReadKeys(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    /* Config the key pins in an input mode away from the LCD of which they are shared with */
    /* note that this must be done before reading the keys as the LCD code will reconfigure these lines */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN |  KEY_DOWN_PIN |  KEY_LEFT_PIN | KEY_RIGHT_PIN | KEY_ENTER_PIN | KEY_CANCEL_PIN;

    RCC_APB2PeriphClockCmd(KEYS_GPIO_CLK, ENABLE);

    GPIO_Init(KEYS_GPIO, &GPIO_InitStructure);

    return GPIO_ReadInputData(KEYS_GPIO) & (KEY_UP_PIN |  KEY_DOWN_PIN |  KEY_LEFT_PIN | KEY_RIGHT_PIN | KEY_ENTER_PIN | KEY_CANCEL_PIN);
}

/* A simple toggle of the Keypad Backlight enable line, additional control may be gained by using the associated DAC */
void KeyBacklightOn(int onoff)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BKLT_ENA_PORT_CLOCKS, ENABLE);

    GPIO_InitStructure.GPIO_Pin = KEY_BKLT_ENA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEY_BKLT_ENA_PORT, &GPIO_InitStructure);

    GPIO_WriteBit(KEY_BKLT_ENA_PORT, KEY_BKLT_ENA_PIN, onoff?Bit_SET:Bit_RESET);
}
