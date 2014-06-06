/*
 * Description:
 *
 * Definition of a simple 1 bit per pixel 8x8 font with the characters
 * starting at 32, a ' '.
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
#ifndef __08x08fnt_h__
#define __08x08fnt_h__
#include <stdint.h>

//Base character is: 32 = 0x20 = " "
#define FONT_08X08_BASE 32
extern const uint8_t Font_08x08[96][8];
extern uint8_t Font_Cyrillic_08x08[256][8];

#endif /* __08x08fnt_h__ */
