/*
 * Description:
 *
 * This defines the layout of the pixels on the LCD glass to the pixels
 * within the LCD controller
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

#ifndef __GLASS_LAYOUT_H__
#define __GLASS_LAYOUT_H__

#define LCD_COLUMN_OFFSET       0
#define LCD_LINE_OFFSET         0

#define LCD_LINES               68
#define LCD_UNMAPPED_LINES      2
#define LCD_COLUMNS             82
#define LCD_PIXELS_PER_COLUMN   3
#define LCD_USABLE_PIXELS_PER_ROW (LCD_PIXELS_ACROSS - LCD_UNMAPPED_LINES)
#define LCD_TEXT_LINE_OFFSET    2
#define LCD_TEXT_COLUMN_OFFSET  2
#define LCD_CHAR_WIDTH          6
#define LCD_CHAR_HEIGHT         8

#define LCD_PIXELS_ACROSS       ( LCD_COLUMNS * LCD_PIXELS_PER_COLUMN )
#define LCD_PIXELS              ( LCD_LINES * LCD_PIXELS_ACROSS )
#define LCD_UNMAPPED_PIXELS     (LCD_UNMAPPED_LINES * LCD_LINES)
#define LCD_BUFFER_BYTE_CNT     (LCD_PIXELS - LCD_UNMAPPED_PIXELS)

#endif /* __GLASS_LAYOUT_H__ */
