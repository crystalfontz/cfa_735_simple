/*
 * Description:
 *
 * Millisecond systick counter
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
#include "stm32f10x.h"

static volatile unsigned int systick = 0;

/* initialize the systick from millisecond interrupts */
void mSecSysTickInit(unsigned int sysfreq)
{
    /* setup the systick as a milisecond tick. */
    SysTick_Config(sysfreq / 1000);
    /* Reprioritize the tick interrupt as a high priority since we are just counting */
    NVIC_SetPriority (SysTick_IRQn, 0);
}

/* get the current systick count */
volatile unsigned int getSysTick_mSecs(void)
{
    return systick;
}

/* increment the tick count each interrupt */
void SysTick_Handler(void)
{
    ++systick;
}
