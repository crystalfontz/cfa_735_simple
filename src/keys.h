/*
 * Description:
 *
 * Function header for keypad reading and key backlight control
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
#ifndef __KEYS_H__
#define __KEYS_H__

#include "platform_config.h"
#include <stdint.h>

/* return the state of the key bus */
uint16_t ReadKeys(void);
/* enable / disable the backlight */
void KeyBacklightOn(int onoff);

#endif // __KEYS_H__
