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
/*
 * Core ST7529 Interface
 */

#include "stm32f10x.h"
#include "st7529_core.h"
#include "glassLayout.h"
#include "platform_config.h"
#include "unistd.h"

// Defines for the interface to the ST7529 LCD Controller
//
typedef unsigned char lcd_bus_width_t;      //  8-bit data bus

#define ST7529_DATA_SET_LSB             0
#define ST7529_DATA_SET_FIELD_LEN       8
#define ST7529_DATA_SET_MASK            (((1 << ST7529_DATA_SET_FIELD_LEN) - 1) << ST7529_DATA_SET_LSB)
#define ST7529_DATA_SET_GET(ST7529WORD) (((ST7529WORD) & ST7529_DATA_SET_MASK) >> ST7529_DATA_SET_LSB)
#define ST7529_DATA_SET_PUT(ST7529WORD) (((ST7529WORD) << ST7529_DATA_SET_LSB) & ST7529_DATA_SET_MASK)


#define ST7529_DATA_RESET_LSB             16
#define ST7529_DATA_RESET_FIELD_LEN       8
#define ST7529_DATA_RESET_MASK            (((1 << ST7529_DATA_RESET_FIELD_LEN) - 1) << ST7529_DATA_RESET_LSB)
#define ST7529_DATA_RESET_GET(ST7529WORD) (((ST7529WORD) & ST7529_DATA_RESET_MASK) >> ST7529_DATA_RESET_LSB)
#define ST7529_DATA_RESET_PUT(ST7529WORD) (((ST7529WORD) << ST7529_DATA_RESET_LSB) & ST7529_DATA_RESET_MASK)


/* ST7529 Command Set */

#define LCD_EXT_SET_0               (lcd_bus_width_t)0x0030
#define LCD_EXT_SET_1               (lcd_bus_width_t)0x0031

/* EXT = 0 Commands
 */
#define LCD_DISPLAY_ON              (lcd_bus_width_t)0x00af
#define LCD_DISPLAY_OFF             (lcd_bus_width_t)0x00ae
#define LCD_DISPLAY_NORMAL          (lcd_bus_width_t)0x00a6
#define LCD_DISPLAY_INVERT          (lcd_bus_width_t)0x00a7
#define LCD_COM_SCAN_DIR            (lcd_bus_width_t)0x00bb
#define LCD_DISPLAY_CONTROL         (lcd_bus_width_t)0x00ca
#define LCD_SLEEP_IN                (lcd_bus_width_t)0x0095
#define LCD_SLEEP_OUT               (lcd_bus_width_t)0x0094
#define LCD_LINE_ADDR_SET           (lcd_bus_width_t)0x0075
#define LCD_COL_ADDR_SET            (lcd_bus_width_t)0x0015
#define LCD_DATA_SCAN_DIR           (lcd_bus_width_t)0x00bc
#define LCD_MEM_WRITE               (lcd_bus_width_t)0x005c
#define LCD_MEM_READ                (lcd_bus_width_t)0x005d
#define LCD_PARTIAL_DISPLAY_IN      (lcd_bus_width_t)0x00a8
#define LCD_PARTIAL_DISPLAY_OUT     (lcd_bus_width_t)0x00a9
#define LCD_READ_MODIFY_WRITE       (lcd_bus_width_t)0x00e0
#define LCD_READ_MODIFY_WRITE_END   (lcd_bus_width_t)0x00ee
#define LCD_AREA_SCROLL_SET         (lcd_bus_width_t)0x00aa
#define LCD_SCROLL_START_SET        (lcd_bus_width_t)0x00ab
#define LCD_INTERNAL_OSC_ON         (lcd_bus_width_t)0x00d1
#define LCD_INTERNAL_OSC_OFF        (lcd_bus_width_t)0x00d2
#define LCD_POWER_CONTROL           (lcd_bus_width_t)0x0020
#define LCD_CONTRAST_CONTROL        (lcd_bus_width_t)0x0081
#define LCD_CONTRAST_INCREASE       (lcd_bus_width_t)0x00d6
#define LCD_CONTRAST_DECREASE       (lcd_bus_width_t)0x00d7
#define LCD_READ_REGISTER_1         (lcd_bus_width_t)0x007c
#define LCD_READ_REGISTER_2         (lcd_bus_width_t)0x007d
#define LCD_NOP                     (lcd_bus_width_t)0x0025
#define LCD_INITIAL_CODE            (lcd_bus_width_t)0x0007

/* EXT = 1 Commands
 */
#define LCD_FRAME_1_GRAY_PWM_SET    (lcd_bus_width_t)0x0020
#define LCD_FRAME_2_GRAY_PWM_SET    (lcd_bus_width_t)0x0021
#define LCD_ANALOG_CIRCUIT_SET      (lcd_bus_width_t)0x0032
#define LCD_SOFTWARE_INITIAL        (lcd_bus_width_t)0x0034
#define LCD_CONTROL_EEPROM          (lcd_bus_width_t)0x00cd
#define LCD_CANCEL_EEPROM           (lcd_bus_width_t)0x00cc
#define LCD_WRITE_EEPROM            (lcd_bus_width_t)0x00fc
#define LCD_READ_EEPROM             (lcd_bus_width_t)0x00fd


/* LCD_CONTRAST_CONTROL command parameter byte 1, 2, and Minimum value
 */
#define LCD_CONTRAST_LSB        0x1a
#define LCD_CONTRAST_MSB        0x04

/* LCD_ANALOG_CIRCUIT_SET Command parameter byte 1
 */
#define OSC_FREQ_ADJ_12_7_KHZ   0b000
#define OSC_FREQ_ADJ_13_2_KHZ   0b100
#define OSC_FREQ_ADJ_14_3_KHZ   0b010
#define OSC_FREQ_ADJ_15_7_KHZ   0b110
#define OSC_FREQ_ADJ_17_3_KHZ   0b001
#define OSC_FREQ_ADJ_19_3_KHZ   0b101
#define OSC_FREQ_ADJ_21_9_KHZ   0b011
#define OSC_FREQ_ADJ_25_4_KHZ   0b111

/* LCD_ANALOG_CIRCUIT_SET Command parameter byte 2
 */
#define BOOSTER_FREQ_SET_3_KHZ  0b00
#define BOOSTER_FREQ_SET_6_KHZ  0b01
#define BOOSTER_FREQ_SET_12_KHZ 0b10
#define BOOSTER_FREQ_SET_24_KHZ 0b11

/* LCD_ANALOG_CIRCUIT_SET Command parameter byte 3
 */
#define LCD_BIAS_RATIO_1_14     0b000
#define LCD_BIAS_RATIO_1_13     0b001
#define LCD_BIAS_RATIO_1_12     0b010
#define LCD_BIAS_RATIO_1_11     0b011
#define LCD_BIAS_RATIO_1_10     0b100
#define LCD_BIAS_RATIO_1_9      0b101
#define LCD_BIAS_RATIO_1_7      0b110
#define LCD_BIAS_RATIO_1_5      0b111

/* LCD_COM_SCAN_DIR Command parameter byte 1
 */
#define COM_0_79__80_159     0       // 0 -> 79, 80 -> 159
#define COM_0_79__159_80     1       // 0 -> 79, 159 -> 80
#define COM_79_0__80_159     2       // 79 -> 0, 80 -> 159
#define COM_79_0__159_80     3       // 79 -> 0, 159 -> 80

#define COM_SCAN_DIR_PB1_CD_LSB                                     0
#define COM_SCAN_DIR_PB1_CD_FIELD_LEN                               3
#define COM_SCAN_DIR_PB1_CD_MASK                                    (((1 << COM_SCAN_DIR_PB1_CD_FIELD_LEN) - 1) << COM_SCAN_DIR_PB1_CD_LSB)
#define COM_SCAN_DIR_PB1_CD_GET(COM_SCAN_DIR_PB1_WORD)              (((COM_SCAN_DIR_PB1_WORD) & COM_SCAN_DIR_PB1_CD_MASK) >> COM_SCAN_DIR_PB1_CD_LSB)
#define COM_SCAN_DIR_PB1_CD_PUT(COM_SCAN_DIR_PB1_WORD)              (((COM_SCAN_DIR_PB1_WORD) << COM_SCAN_DIR_PB1_CD_LSB) & COM_SCAN_DIR_PB1_CD_MASK)

/* LCD_DSIPLAY_CONTROL Command Parameter Bytes 1, 2, & 3
 */
#define CLD_NO_DIVIDE   0
#define CLD_DIVIDE_2    1
#define DT              ((LCD_LINES / 4) - 1)
#define LF              15
#define FI              1

#define DISPLAY_CONTROL_PB1_CLD_LSB                                 2
#define DISPLAY_CONTROL_PB1_CLD_FIELD_LEN                           1
#define DISPLAY_CONTROL_PB1_CLD_MASK                                (((1 << DISPLAY_CONTROL_PB1_CLD_FIELD_LEN) - 1) << DISPLAY_CONTROL_PB1_CLD_LSB)
#define DISPLAY_CONTROL_PB1_CLD_GET(DISPLAY_CONTROL_PB1_WORD)       (((DISPLAY_CONTROL_PB1_WORD) & DISPLAY_CONTROL_PB1_CLD_MASK) >> DISPLAY_CONTROL_PB1_CLD_LSB)
#define DISPLAY_CONTROL_PB1_CLD_PUT(DISPLAY_CONTROL_PB1_WORD)       (((DISPLAY_CONTROL_PB1_WORD) << DISPLAY_CONTROL_PB1_CLD_LSB) & DISPLAY_CONTROL_PB1_CLD_MASK)

#define DISPLAY_CONTROL_PB2_DT_LSB                                  0
#define DISPLAY_CONTROL_PB2_DT_FIELD_LEN                            6
#define DISPLAY_CONTROL_PB2_DT_MASK                                 (((1 << DISPLAY_CONTROL_PB2_DT_FIELD_LEN) - 1) << DISPLAY_CONTROL_PB2_DT_LSB)
#define DISPLAY_CONTROL_PB2_DT_GET(DISPLAY_CONTROL_PB2_WORD)        (((DISPLAY_CONTROL_PB2_WORD) & DISPLAY_CONTROL_PB2_DT_MASK) >> DISPLAY_CONTROL_PB2_DT_LSB)
#define DISPLAY_CONTROL_PB2_DT_PUT(DISPLAY_CONTROL_PB2_WORD)        (((DISPLAY_CONTROL_PB2_WORD) << DISPLAY_CONTROL_PB2_DT_LSB) & DISPLAY_CONTROL_PB2_DT_MASK)

#define DISPLAY_CONTROL_PB3_LF_LSB                                  0
#define DISPLAY_CONTROL_PB3_LF_FIELD_LEN                            4
#define DISPLAY_CONTROL_PB3_LF_MASK                                 (((1 << DISPLAY_CONTROL_PB3_LF_FIELD_LEN) - 1) << DISPLAY_CONTROL_PB3_LF_LSB)
#define DISPLAY_CONTROL_PB3_LF_GET(DISPLAY_CONTROL_PB3_WORD)        (((DISPLAY_CONTROL_PB3_WORD) & DISPLAY_CONTROL_PB3_LF_MASK) >> DISPLAY_CONTROL_PB3_LF_LSB)
#define DISPLAY_CONTROL_PB3_LF_PUT(DISPLAY_CONTROL_PB3_WORD)        (((DISPLAY_CONTROL_PB3_WORD) << DISPLAY_CONTROL_PB3_LF_LSB) & DISPLAY_CONTROL_PB3_LF_MASK)

#define DISPLAY_CONTROL_PB3_FI_LSB                                  4
#define DISPLAY_CONTROL_PB3_FI_FIELD_LEN                            1
#define DISPLAY_CONTROL_PB3_FI_MASK                                 (((1 << DISPLAY_CONTROL_PB3_FI_FIELD_LEN) - 1) << DISPLAY_CONTROL_PB3_FI_LSB)
#define DISPLAY_CONTROL_PB3_FI_GET(DISPLAY_CONTROL_PB3_WORD)        (((DISPLAY_CONTROL_PB3_WORD) & DISPLAY_CONTROL_PB3_FI_MASK) >> DISPLAY_CONTROL_PB3_FI_LSB)
#define DISPLAY_CONTROL_PB3_FI_PUT(DISPLAY_CONTROL_PB3_WORD)        (((DISPLAY_CONTROL_PB3_WORD) << DISPLAY_CONTROL_PB3_FI_LSB) & DISPLAY_CONTROL_PB3_FI_MASK)



/* LCD_LINE_ADDR_SET Command Parameter Bytes 1 & 2
 */
#define LINE_ADDR_SET_PB1_START_LINE_LSB                            0
#define LINE_ADDR_SET_PB1_START_LINE_FIELD_LEN                      8
#define LINE_ADDR_SET_PB1_START_LINE_MASK                           (((1 << LINE_ADDR_SET_PB1_START_LINE_FIELD_LEN) - 1) << LINE_ADDR_SET_PB1_START_LINE_LSB)
#define LINE_ADDR_SET_PB1_START_LINE_GET(LINE_ADDR_SET_PB1_WORD)    (((LINE_ADDR_SET_PB1_WORD) & LINE_ADDR_SET_PB1_START_LINE_MASK) >> LINE_ADDR_SET_PB1_START_LINE_LSB)
#define LINE_ADDR_SET_PB1_START_LINE_PUT(LINE_ADDR_SET_PB1_WORD)    (((LINE_ADDR_SET_PB1_WORD) << LINE_ADDR_SET_PB1_START_LINE_LSB) & LINE_ADDR_SET_PB1_START_LINE_MASK)

#define LINE_ADDR_SET_PB2_END_LINE_LSB                              0
#define LINE_ADDR_SET_PB2_END_LINE_FIELD_LEN                        8
#define LINE_ADDR_SET_PB2_END_LINE_MASK                             (((1 << LINE_ADDR_SET_PB2_END_LINE_FIELD_LEN) - 1) << LINE_ADDR_SET_PB2_END_LINE_LSB)
#define LINE_ADDR_SET_PB2_END_LINE_GET(LINE_ADDR_SET_PB2_WORD)      (((LINE_ADDR_SET_PB2_WORD) & LINE_ADDR_SET_PB2_END_LINE_MASK) >> LINE_ADDR_SET_PB2_END_LINE_LSB)
#define LINE_ADDR_SET_PB2_END_LINE_PUT(LINE_ADDR_SET_PB2_WORD)      (((LINE_ADDR_SET_PB2_WORD) << LINE_ADDR_SET_PB2_END_LINE_LSB) & LINE_ADDR_SET_PB2_END_LINE_MASK)



/* LCD_COL_ADDR_SET Command parameter bytes 1 & 2
 */
#define COL_ADDR_SET_PB1_START_COL_LSB                              0
#define COL_ADDR_SET_PB1_START_COL_FIELD_LEN                        8
#define COL_ADDR_SET_PB1_START_COL_MASK                             (((1 << COL_ADDR_SET_PB1_START_COL_FIELD_LEN) - 1) << COL_ADDR_SET_PB1_START_COL_LSB)
#define COL_ADDR_SET_PB1_START_COL_GET(COL_ADDR_SET_PB1_WORD)       (((COL_ADDR_SET_PB1_WORD) & COL_ADDR_SET_PB1_START_COL_MASK) >> COL_ADDR_SET_PB1_START_COL_LSB)
#define COL_ADDR_SET_PB1_START_COL_PUT(COL_ADDR_SET_PB1_WORD)       (((COL_ADDR_SET_PB1_WORD) << COL_ADDR_SET_PB1_START_COL_LSB) & COL_ADDR_SET_PB1_START_COL_MASK)

#define COL_ADDR_SET_PB2_END_COL_LSB                                0
#define COL_ADDR_SET_PB2_END_COL_FIELD_LEN                          8
#define COL_ADDR_SET_PB2_END_COL_MASK                               (((1 << COL_ADDR_SET_PB2_END_COL_FIELD_LEN) - 1) << COL_ADDR_SET_PB2_END_COL_LSB)
#define COL_ADDR_SET_PB2_END_COL_GET(COL_ADDR_SET_PB2_WORD)         (((COL_ADDR_SET_PB2_WORD) & COL_ADDR_SET_PB2_END_COL_MASK) >> COL_ADDR_SET_PB2_END_COL_LSB)
#define COL_ADDR_SET_PB2_END_COL_PUT(COL_ADDR_SET_PB2_WORD)         (((COL_ADDR_SET_PB2_WORD) << COL_ADDR_SET_PB2_END_COL_LSB) & COL_ADDR_SET_PB2_END_COL_MASK)


/* LCD_DATA_SCAN_DIR Command parameter bytes 1, 2, & 3
 */
#define LI_NORMAL   0
#define LI_INVERSE  1
#define CI_NORMAL   0
#define CI_INVERSE  1
#define C_L_COLUMN_DIRECTION    0
#define C_L_LINE_DIRECTION      1
#define CLR_NORMAL  0
#define CLR_REVERSE 1
#define GS_2BYTE_3PIXEL 1
#define GS_3BYTE_3PIXEL 2


#define DATA_SCAN_DIR_PB1_LI_LSB                                    0
#define DATA_SCAN_DIR_PB1_LI_FIELD_LEN                              1
#define DATA_SCAN_DIR_PB1_LI_MASK                                   (((1 << DATA_SCAN_DIR_PB1_LI_FIELD_LEN) - 1) << DATA_SCAN_DIR_PB1_LI_LSB)
#define DATA_SCAN_DIR_PB1_LI_GET(DATA_SCAN_DIR_PB1_WORD)            (((DATA_SCAN_DIR_PB1_WORD) & DATA_SCAN_DIR_PB1_LI_MASK) >> DATA_SCAN_DIR_PB1_LI_LSB)
#define DATA_SCAN_DIR_PB1_LI_PUT(DATA_SCAN_DIR_PB1_WORD)            (((DATA_SCAN_DIR_PB1_WORD) << DATA_SCAN_DIR_PB1_LI_LSB) & DATA_SCAN_DIR_PB1_LI_MASK)

#define DATA_SCAN_DIR_PB1_CI_LSB                                    1
#define DATA_SCAN_DIR_PB1_CI_FIELD_LEN                              1
#define DATA_SCAN_DIR_PB1_CI_MASK                                   (((1 << DATA_SCAN_DIR_PB1_CI_FIELD_LEN) - 1) << DATA_SCAN_DIR_PB1_CI_LSB)
#define DATA_SCAN_DIR_PB1_CI_GET(DATA_SCAN_DIR_PB1_WORD)            (((DATA_SCAN_DIR_PB1_WORD) & DATA_SCAN_DIR_PB1_CI_MASK) >> DATA_SCAN_DIR_PB1_CI_LSB)
#define DATA_SCAN_DIR_PB1_CI_PUT(DATA_SCAN_DIR_PB1_WORD)            (((DATA_SCAN_DIR_PB1_WORD) << DATA_SCAN_DIR_PB1_CI_LSB) & DATA_SCAN_DIR_PB1_CI_MASK)

#define DATA_SCAN_DIR_PB1_C_L_LSB                                   2
#define DATA_SCAN_DIR_PB1_C_L_FIELD_LEN                             1
#define DATA_SCAN_DIR_PB1_C_L_MASK                                  (((1 << DATA_SCAN_DIR_PB1_C_L_FIELD_LEN) - 1) << DATA_SCAN_DIR_PB1_C_L_LSB)
#define DATA_SCAN_DIR_PB1_C_L_GET(DATA_SCAN_DIR_PB1_WORD)           (((DATA_SCAN_DIR_PB1_WORD) & DATA_SCAN_DIR_PB1_C_L_MASK) >> DATA_SCAN_DIR_PB1_C_L_LSB)
#define DATA_SCAN_DIR_PB1_C_L_PUT(DATA_SCAN_DIR_PB1_WORD)           (((DATA_SCAN_DIR_PB1_WORD) << DATA_SCAN_DIR_PB1_C_L_LSB) & DATA_SCAN_DIR_PB1_C_L_MASK)

#define DATA_SCAN_DIR_PB2_CLR_LSB                                   0
#define DATA_SCAN_DIR_PB2_CLR_FIELD_LEN                             1
#define DATA_SCAN_DIR_PB2_CLR_MASK                                  (((1 << DATA_SCAN_DIR_PB2_CLR_FIELD_LEN) - 1) << DATA_SCAN_DIR_PB2_CLR_LSB)
#define DATA_SCAN_DIR_PB2_CLR_GET(DATA_SCAN_DIR_PB2_WORD)           (((DATA_SCAN_DIR_PB2_WORD) & DATA_SCAN_DIR_PB2_CLR_MASK) >> DATA_SCAN_DIR_PB2_CLR_LSB)
#define DATA_SCAN_DIR_PB2_CLR_PUT(DATA_SCAN_DIR_PB2_WORD)           (((DATA_SCAN_DIR_PB2_WORD) << DATA_SCAN_DIR_PB2_CLR_LSB) & DATA_SCAN_DIR_PB2_CLR_MASK)

#define DATA_SCAN_DIR_PB3_GS_LSB                                    0
#define DATA_SCAN_DIR_PB3_GS_FIELD_LEN                              3
#define DATA_SCAN_DIR_PB3_GS_MASK                                   (((1 << DATA_SCAN_DIR_PB3_GS_FIELD_LEN) - 1) << DATA_SCAN_DIR_PB3_GS_LSB)
#define DATA_SCAN_DIR_PB3_GS_GET(DATA_SCAN_DIR_PB3_WORD)            (((DATA_SCAN_DIR_PB3_WORD) & DATA_SCAN_DIR_PB3_GS_MASK) >> DATA_SCAN_DIR_PB3_GS_LSB)
#define DATA_SCAN_DIR_PB3_GS_PUT(DATA_SCAN_DIR_PB3_WORD)            (((DATA_SCAN_DIR_PB3_WORD) << DATA_SCAN_DIR_PB3_GS_LSB) & DATA_SCAN_DIR_PB3_GS_MASK)


/* LCD_MEM_WRITE Command parameter byte/halfword 1
 */
#define MEM_WRITE_PB1_DATA_LSB                                      0
#define MEM_WRITE_PB1_DATA_FIELD_LEN                                sizeof(lcd_bus_width_t)
#define MEM_WRITE_PB1_DATA_MASK                                     (((1 << MEM_WRITE_PB1_DATA_FIELD_LEN) - 1) << MEM_WRITE_PB1_DATA_LSB)
#define MEM_WRITE_PB1_DATA_GET(MEM_WRITE_PB1_WORD)                  (((MEM_WRITE_PB1_WORD) & MEM_WRITE_PB1_DATA_MASK) >> MEM_WRITE_PB1_DATA_LSB)
#define MEM_WRITE_PB1_DATA_PUT(MEM_WRITE_PB1_WORD)                  (((MEM_WRITE_PB1_WORD) << MEM_WRITE_PB1_DATA_LSB) & MEM_WRITE_PB1_DATA_MASK)

/* LCD_MEM_READ Command parameter byte/halfword 1
 */
#define MEM_READ_PB1_DATA_LSB                                       0
#define MEM_READ_PB1_DATA_FIELD_LEN                                 sizeof(lcd_bus_width_t)
#define MEM_READ_PB1_DATA_MASK                                      (((1 << MEM_READ_PB1_DATA_FIELD_LEN) - 1) << MEM_READ_PB1_DATA_LSB)
#define MEM_READ_PB1_DATA_GET(MEM_READ_PB1_WORD)                    (((MEM_READ_PB1_WORD) & MEM_READ_PB1_DATA_MASK) >> MEM_READ_PB1_DATA_LSB)
#define MEM_READ_PB1_DATA_PUT(MEM_READ_PB1_WORD)                    (((MEM_READ_PB1_WORD) << MEM_READ_PB1_DATA_LSB) & MEM_READ_PB1_DATA_MASK)

/* LCD_PARTIAL_DISPLAY_IN Command parameter bytes 1 & 2
 */
#define PARTIAL_DISPLAY_IN_PB1_PTS_LSB                              0
#define PARTIAL_DISPLAY_IN_PB1_PTS_FIELD_LEN                        6
#define PARTIAL_DISPLAY_IN_PB1_PTS_MASK                             (((1 << PARTIAL_DISPLAY_IN_PB1_PTS_FIELD_LEN) - 1) << PARTIAL_DISPLAY_IN_PB1_PTS_LSB)
#define PARTIAL_DISPLAY_IN_PB1_PTS_GET(PARTIAL_DISPLAY_IN_PB1_WORD) (((PARTIAL_DISPLAY_IN_PB1_WORD) & PARTIAL_DISPLAY_IN_PB1_PTS_MASK) >> PARTIAL_DISPLAY_IN_PB1_PTS_LSB)
#define PARTIAL_DISPLAY_IN_PB1_PTS_PUT(PARTIAL_DISPLAY_IN_PB1_WORD) (((PARTIAL_DISPLAY_IN_PB1_WORD) << PARTIAL_DISPLAY_IN_PB1_PTS_LSB) & PARTIAL_DISPLAY_IN_PB1_PTS_MASK)

#define PARTIAL_DISPLAY_IN_PB2_PTE_LSB                              0
#define PARTIAL_DISPLAY_IN_PB2_PTE_FIELD_LEN                        6
#define PARTIAL_DISPLAY_IN_PB2_PTE_MASK                             (((1 << PARTIAL_DISPLAY_IN_PB2_PTE_FIELD_LEN) - 1) << PARTIAL_DISPLAY_IN_PB2_PTE_LSB)
#define PARTIAL_DISPLAY_IN_PB2_PTE_GET(PARTIAL_DISPLAY_IN_PB2_WORD) (((PARTIAL_DISPLAY_IN_PB2_WORD) & PARTIAL_DISPLAY_IN_PB2_PTE_MASK) >> PARTIAL_DISPLAY_IN_PB2_PTE_LSB)
#define PARTIAL_DISPLAY_IN_PB2_PTE_PUT(PARTIAL_DISPLAY_IN_PB2_WORD) (((PARTIAL_DISPLAY_IN_PB2_WORD) << PARTIAL_DISPLAY_IN_PB2_PTE_LSB) & PARTIAL_DISPLAY_IN_PB2_PTE_MASK)

/* LCD_AREA_SCROLL_SET Command parameter bytes 1, 2, 3, & 4
 */
#define AREA_SCROLL_SET_PB1_TB_LSB                                  0
#define AREA_SCROLL_SET_PB1_TB_FIELD_LEN                            6
#define AREA_SCROLL_SET_PB1_TB_MASK                                 (((1 << AREA_SCROLL_SET_PB1_TB_FIELD_LEN) - 1) << AREA_SCROLL_SET_PB1_TB_LSB)
#define AREA_SCROLL_SET_PB1_TB_GET(AREA_SCROLL_SET_PB1_WORD)        (((AREA_SCROLL_SET_PB1_WORD) & AREA_SCROLL_SET_PB1_TB_MASK) >> AREA_SCROLL_SET_PB1_TB_LSB)
#define AREA_SCROLL_SET_PB1_TB_PUT(AREA_SCROLL_SET_PB1_WORD)        (((AREA_SCROLL_SET_PB1_WORD) << AREA_SCROLL_SET_PB1_TB_LSB) & AREA_SCROLL_SET_PB1_TB_MASK)

#define AREA_SCROLL_SET_PB2_BB_LSB                                  0
#define AREA_SCROLL_SET_PB2_BB_FIELD_LEN                            6
#define AREA_SCROLL_SET_PB2_BB_MASK                                 (((1 << AREA_SCROLL_SET_PB2_BB_FIELD_LEN) - 1) << AREA_SCROLL_SET_PB2_BB_LSB)
#define AREA_SCROLL_SET_PB2_BB_GET(AREA_SCROLL_SET_PB2_WORD)        (((AREA_SCROLL_SET_PB2_WORD) & AREA_SCROLL_SET_PB2_BB_MASK) >> AREA_SCROLL_SET_PB2_BB_LSB)
#define AREA_SCROLL_SET_PB2_BB_PUT(AREA_SCROLL_SET_PB2_WORD)        (((AREA_SCROLL_SET_PB2_WORD) << AREA_SCROLL_SET_PB2_BB_LSB) & AREA_SCROLL_SET_PB2_BB_MASK)

#define AREA_SCROLL_SET_PB3_NSB_LSB                                 0
#define AREA_SCROLL_SET_PB3_NSB_FIELD_LEN                           6
#define AREA_SCROLL_SET_PB3_NSB_MASK                                (((1 << AREA_SCROLL_SET_PB3_NSB_FIELD_LEN) - 1) << AREA_SCROLL_SET_PB3_NSB_LSB)
#define AREA_SCROLL_SET_PB3_NSB_GET(AREA_SCROLL_SET_PB3_WORD)       (((AREA_SCROLL_SET_PB3_WORD) & AREA_SCROLL_SET_PB3_NSB_MASK) >> AREA_SCROLL_SET_PB3_NSB_LSB)
#define AREA_SCROLL_SET_PB3_NSB_PUT(AREA_SCROLL_SET_PB3_WORD)       (((AREA_SCROLL_SET_PB3_WORD) << AREA_SCROLL_SET_PB3_NSB_LSB) & AREA_SCROLL_SET_PB3_NSB_MASK)

#define AREA_SCROLL_SET_PB4_SCM_LSB                                 0
#define AREA_SCROLL_SET_PB4_SCM_FIELD_LEN                           2
#define AREA_SCROLL_SET_PB4_SCM_MASK                                (((1 << AREA_SCROLL_SET_PB4_SCM_FIELD_LEN) - 1) << AREA_SCROLL_SET_PB4_SCM_LSB)
#define AREA_SCROLL_SET_PB4_SCM_GET(AREA_SCROLL_SET_PB4_WORD)       (((AREA_SCROLL_SET_PB4_WORD) & AREA_SCROLL_SET_PB4_SCM_MASK) >> AREA_SCROLL_SET_PB4_SCM_LSB)
#define AREA_SCROLL_SET_PB4_SCM_PUT(AREA_SCROLL_SET_PB4_WORD)       (((AREA_SCROLL_SET_PB4_WORD) << AREA_SCROLL_SET_PB4_SCM_LSB) & AREA_SCROLL_SET_PB4_SCM_MASK)


/* LCD_START_SCROLL_SET Command parameter byte 1
 */
#define START_SCROLL_SET_PB1_SB_LSB                                 0
#define START_SCROLL_SET_PB1_SB_FIELD_LEN                           6
#define START_SCROLL_SET_PB1_SB_MASK                                (((1 << START_SCROLL_SET_PB1_SB_FIELD_LEN) - 1) << START_SCROLL_SET_PB1_SB_LSB)
#define START_SCROLL_SET_PB1_SB_GET(START_SCROLL_SET_PB1_WORD)      (((START_SCROLL_SET_PB1_WORD) & START_SCROLL_SET_PB1_SB_MASK) >> START_SCROLL_SET_PB1_SB_LSB)
#define START_SCROLL_SET_PB1_SB_PUT(START_SCROLL_SET_PB1_WORD)      (((START_SCROLL_SET_PB1_WORD) << START_SCROLL_SET_PB1_SB_LSB) & START_SCROLL_SET_PB1_SB_MASK)


/* LCD_POWER_CONTROL Command parameter byte 1
 */
#define VR_OFF  0   // Voltage reference generator
#define VR_ON   1
#define VF_OFF  0   // Voltage follower
#define VF_ON   1
#define VB_OFF  0   // Voltage boost
#define VB_ON   1

#define POWER_CONTROL_PB1_VR_LSB                                    0
#define POWER_CONTROL_PB1_VR_FIELD_LEN                              1
#define POWER_CONTROL_PB1_VR_MASK                                   (((1 << POWER_CONTROL_PB1_VR_FIELD_LEN) - 1) << POWER_CONTROL_PB1_VR_LSB)
#define POWER_CONTROL_PB1_VR_GET(POWER_CONTROL_PB1_WORD)            (((POWER_CONTROL_PB1_WORD) & POWER_CONTROL_PB1_VR_MASK) >> POWER_CONTROL_PB1_VR_LSB)
#define POWER_CONTROL_PB1_VR_PUT(POWER_CONTROL_PB1_WORD)            (((POWER_CONTROL_PB1_WORD) << POWER_CONTROL_PB1_VR_LSB) & POWER_CONTROL_PB1_VR_MASK)

#define POWER_CONTROL_PB1_VF_LSB                                    1
#define POWER_CONTROL_PB1_VF_FIELD_LEN                              1
#define POWER_CONTROL_PB1_VF_MASK                                   (((1 << POWER_CONTROL_PB1_VF_FIELD_LEN) - 1) << POWER_CONTROL_PB1_VF_LSB)
#define POWER_CONTROL_PB1_VF_GET(POWER_CONTROL_PB1_WORD)            (((POWER_CONTROL_PB1_WORD) & POWER_CONTROL_PB1_VF_MASK) >> POWER_CONTROL_PB1_VF_LSB)
#define POWER_CONTROL_PB1_VF_PUT(POWER_CONTROL_PB1_WORD)            (((POWER_CONTROL_PB1_WORD) << POWER_CONTROL_PB1_VF_LSB) & POWER_CONTROL_PB1_VF_MASK)

#define POWER_CONTROL_PB1_VB_LSB                                    3
#define POWER_CONTROL_PB1_VB_FIELD_LEN                              1
#define POWER_CONTROL_PB1_VB_MASK                                   (((1 << POWER_CONTROL_PB1_VB_FIELD_LEN) - 1) << POWER_CONTROL_PB1_VB_LSB)
#define POWER_CONTROL_PB1_VB_GET(POWER_CONTROL_PB1_WORD)            (((POWER_CONTROL_PB1_WORD) & POWER_CONTROL_PB1_VB_MASK) >> POWER_CONTROL_PB1_VB_LSB)
#define POWER_CONTROL_PB1_VB_PUT(POWER_CONTROL_PB1_WORD)            (((POWER_CONTROL_PB1_WORD) << POWER_CONTROL_PB1_VB_LSB) & POWER_CONTROL_PB1_VB_MASK)

/* LCD_CONTRAST_CONTROL Command parameter bytes 1 & 2
 */
#define CONTRAST_CONTROL_PB1_VPR_5_0_LSB                            0
#define CONTRAST_CONTROL_PB1_VPR_5_0_FIELD_LEN                      6
#define CONTRAST_CONTROL_PB1_VPR_5_0_MASK                           (((1 << CONTRAST_CONTROL_PB1_VPR_5_0_FIELD_LEN) - 1) << CONTRAST_CONTROL_PB1_VPR_5_0_LSB)
#define CONTRAST_CONTROL_PB1_VPR_5_0_GET(CONTRAST_CONTROL_PB1_WORD) (((CONTRAST_CONTROL_PB1_WORD) & CONTRAST_CONTROL_PB1_VPR_5_0_MASK) >> CONTRAST_CONTROL_PB1_VPR_5_0_LSB)
#define CONTRAST_CONTROL_PB1_VPR_5_0_PUT(CONTRAST_CONTROL_PB1_WORD) (((CONTRAST_CONTROL_PB1_WORD) << CONTRAST_CONTROL_PB1_VPR_5_0_LSB) & CONTRAST_CONTROL_PB1_VPR_5_0_MASK)

#define CONTRAST_CONTROL_PB2_VPR_8_6_LSB                            0
#define CONTRAST_CONTROL_PB2_VPR_8_6_FIELD_LEN                      3
#define CONTRAST_CONTROL_PB2_VPR_8_6_MASK                           (((1 << CONTRAST_CONTROL_PB2_VPR_8_6_FIELD_LEN) - 1) << CONTRAST_CONTROL_PB2_VPR_8_6_LSB)
#define CONTRAST_CONTROL_PB2_VPR_8_6_GET(CONTRAST_CONTROL_PB2_WORD) (((CONTRAST_CONTROL_PB2_WORD) & CONTRAST_CONTROL_PB2_VPR_8_6_MASK) >> CONTRAST_CONTROL_PB2_VPR_8_6_LSB)
#define CONTRAST_CONTROL_PB2_VPR_8_6_PUT(CONTRAST_CONTROL_PB2_WORD) (((CONTRAST_CONTROL_PB2_WORD) << CONTRAST_CONTROL_PB2_VPR_8_6_LSB) & CONTRAST_CONTROL_PB2_VPR_8_6_MASK)


static inline void ST7529_rdLOW() { ST7529_nRD_GPIO->BRR = ST7529_nRD; }
static inline void ST7529_rdHIGH() { ST7529_nRD_GPIO->BSRR = ST7529_nRD; }
static inline void ST7529_wrLOW() { ST7529_nWR_GPIO->BRR = ST7529_nWR; }
static inline void ST7529_wrHIGH() { ST7529_nWR_GPIO->BSRR = ST7529_nWR; }
static inline void ST7529_csLOW() { ST7529_nCS_GPIO->BRR = ST7529_nCS; }
static inline void ST7529_csHIGH() { ST7529_nCS_GPIO->BSRR = ST7529_nCS; }
static inline void ST7529_a0LOW() { ST7529_A0_GPIO->BRR = ST7529_A0; }
static inline void ST7529_a0HIGH() { ST7529_A0_GPIO->BSRR = ST7529_A0; }
static inline void ST7529_rstLOW() { GPIO_ResetBits(ST7529_nRST_GPIO, ST7529_nRST); }
static inline void ST7529_rstHIGH() { GPIO_SetBits(ST7529_nRST_GPIO, ST7529_nRST); }

static void ST7529_writeDataByte(uint8_t ST7529_Byte);
static void ST7529_writeCMD( uint8_t cmd );
static void ST7529_writeDATA( uint8_t data );
static void ST7529_writeLineAddr( uint8_t startLine, uint8_t endLine );
static void ST7529_writeColAddr( uint8_t startCol, uint8_t endCol );
static void ST7529_loadContrastAdjFromEEPROM( void );

/* init the pins on the LCD data bus for output */
void ST7529_busInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    // Configure I/O for ST7529 8-bit data bus
    GPIO_InitStructure.GPIO_Pin = ST7529_DATA_PINS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(ST7529_DATA_GPIO, &GPIO_InitStructure);
}

/* init the pins on the LCD data bus for intput */
void ST7529_busToRead(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    // Configure I/O for ST7529 8-bit data bus
    GPIO_InitStructure.GPIO_Pin = ST7529_DATA_PINS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(ST7529_DATA_GPIO, &GPIO_InitStructure);
}


/* Config the STM32 to talk to the ST7529 and init the ST7529 */
void ST7529_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    // Enable ST7529 GPIO peripherals
    RCC_APB2PeriphClockCmd(ST7529_DATA_GPIO_CLK | ST7529_nCS_GPIO_CLK
                           | ST7529_A0_GPIO_CLK | ST7529_nRST_GPIO_CLK | ST7529_nWR_GPIO_CLK, ENABLE);

    ST7529_busInit();

    // Configure I/O for ST7529 A0 and nCS
    GPIO_InitStructure.GPIO_Pin = ST7529_A0 | ST7529_nCS;
    GPIO_Init(ST7529_A0_GPIO, &GPIO_InitStructure);

    // Configure I/O for ST7529 nRD and nWR
    GPIO_InitStructure.GPIO_Pin = ST7529_nRD | ST7529_nWR;
    GPIO_Init(ST7529_nRD_GPIO, &GPIO_InitStructure);

    /* Configure I/O for ST7529 nRST */
    GPIO_InitStructure.GPIO_Pin = ST7529_nRST;
    GPIO_Init(ST7529_nRST_GPIO, &GPIO_InitStructure);

    // Deselect the ST7529: Chip Select high
    ST7529_rdHIGH();
    ST7529_wrHIGH();
    ST7529_csHIGH();
    ST7529_a0LOW();

    /* Reset the LCD
     */
    ST7529_rstLOW();
    usleep(15000);
    ST7529_rstHIGH();
    usleep(15000);


    ST7529_writeCMD(LCD_NOP);
    ST7529_writeCMD(LCD_NOP);

    /* Ext = 0 Commands to be written
     * Sleet Out
     * Oscillator ON
     */
    ST7529_writeCMD(LCD_EXT_SET_0);
    ST7529_writeCMD(LCD_SLEEP_OUT);
    ST7529_writeCMD(LCD_INTERNAL_OSC_ON);

    /* Power Control Set
     * Booster must be ON first
     */
    ST7529_writeCMD(LCD_POWER_CONTROL);
    ST7529_writeDATA(POWER_CONTROL_PB1_VB_PUT(VB_ON));
    usleep(20000);

    /* Power Control Set
     * Booster, Regulator and Follower ON
     */
    ST7529_writeCMD(LCD_POWER_CONTROL);
    usleep(1000);
    ST7529_writeDATA(POWER_CONTROL_PB1_VB_PUT(VB_ON)
              | POWER_CONTROL_PB1_VR_PUT(VR_ON)
              | POWER_CONTROL_PB1_VF_PUT(VF_ON));
    usleep(5000);

    /* Initial Contrast Values
     */
    ST7529_writeContrast(LCD_CONTRAST_INIT);

    /* Display control
     */
    ST7529_writeCMD(LCD_DISPLAY_CONTROL);
    ST7529_writeDATA(DISPLAY_CONTROL_PB1_CLD_PUT(CLD_DIVIDE_2));
    ST7529_writeDATA(0x1c);
    ST7529_writeDATA(0Xa);

    ST7529_writeCMD(LCD_DISPLAY_INVERT);

    /* Set the COMs scan direction
     */
    ST7529_writeCMD(LCD_COM_SCAN_DIR);
    ST7529_writeDATA(COM_SCAN_DIR_PB1_CD_PUT( COM_79_0__80_159));

    /* Set the data scan direction
     */
    ST7529_writeCMD(LCD_DATA_SCAN_DIR);
    ST7529_writeDATA(DATA_SCAN_DIR_PB1_LI_PUT(LI_INVERSE)                  // NORMAL
              | DATA_SCAN_DIR_PB1_CI_PUT(CI_NORMAL)                 // INVERSE
              | DATA_SCAN_DIR_PB1_C_L_PUT(C_L_COLUMN_DIRECTION));   // LINE
    ST7529_writeDATA(DATA_SCAN_DIR_PB2_CLR_PUT(CLR_NORMAL));               // REVERSE
    ST7529_writeDATA(DATA_SCAN_DIR_PB3_GS_PUT(GS_3BYTE_3PIXEL));

    /* Set the start and end line registers
     */
    ST7529_writeCMD(LCD_LINE_ADDR_SET);
    ST7529_writeDATA(0);
    ST7529_writeDATA(0x71);

    ST7529_writeDATA(LINE_ADDR_SET_PB1_START_LINE_PUT(0 + LCD_LINE_OFFSET));
    ST7529_writeDATA(LINE_ADDR_SET_PB2_END_LINE_PUT(LCD_LINES - 1 + LCD_LINE_OFFSET));

    /* Set the start and end column registers
     */
    ST7529_writeCMD(LCD_COL_ADDR_SET);
    ST7529_writeDATA(COL_ADDR_SET_PB1_START_COL_PUT(0 + LCD_COLUMN_OFFSET));
    ST7529_writeDATA(COL_ADDR_SET_PB2_END_COL_PUT(LCD_COLUMNS - 1 + LCD_COLUMN_OFFSET));
    usleep(1000);

    /* Executing EXT=1 Commands
     * Set the OSC frequency, FR = 12.7KHz
     * Set the Booster Efficiency, Booster FR = 6KHz
     * Set the Bias, 1/9 Bias
     */
    ST7529_writeCMD(LCD_EXT_SET_1);
    ST7529_writeCMD(LCD_ANALOG_CIRCUIT_SET);
    ST7529_writeDATA(OSC_FREQ_ADJ_12_7_KHZ);
    ST7529_writeDATA(BOOSTER_FREQ_SET_6_KHZ);
    ST7529_writeDATA(LCD_BIAS_RATIO_1_9);

    /* Software initialize and EXT=0 Commands to follow
     */
    ST7529_writeCMD(LCD_SOFTWARE_INITIAL);

    ST7529_loadContrastAdjFromEEPROM();

    ST7529_writeCMD(LCD_EXT_SET_0);

    ST7529_writeCMD(LCD_DISPLAY_ON);
}

/* Set data on the lines to the ST7529*/
static void ST7529_writeDataByte(uint8_t ST7529_Byte)
{
    // When writing to the BSSR register, the SET bits have priority over the RESET
    // bits. Therefore, we write 1 to ALL 8 reset bits and 1 to the appropriate SET bits
    // and end up with the correct byte written to the ST7529 data pins.
    ST7529_DATA_GPIO->BSRR = (uint32_t)(ST7529_Byte) | (uint32_t)(ST7529_DATA_RESET_MASK);
}

/* Write a command byte to the ST7529 */
static void ST7529_writeCMD( uint8_t cmd )
{
    ST7529_a0LOW();
    ST7529_writeDataByte(cmd);
    ST7529_csLOW();
    ST7529_wrLOW();
    // Required to meet ST7529 TDS8 (Write data setup time) of >= 150nS.
    // Measured TDS8 = 160nS.
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    ST7529_wrHIGH();
    ST7529_csHIGH();
    ST7529_a0HIGH();
}

/* Write a data byte to the ST7529 */
static void ST7529_writeDATA( uint8_t data )
{
    ST7529_writeDataByte(data);
    ST7529_csLOW();
    ST7529_wrLOW();
    // Required to meet ST7529 TDS8 (Write data setup time) of >= 150nS.
    // Measured TDS8 = 160nS.
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    ST7529_wrHIGH();
    ST7529_csHIGH();
}

/* read a data byte from the ST7529 */
static uint8_t ST7529_readREG( void )
{
    uint8_t val;
    ST7529_busToRead();
    ST7529_a0LOW();
    ST7529_csLOW();
    ST7529_rdLOW();
    // Required to meet ST7529 TDS8 (Read data setup time) of >= 150nS.
    // Measured TDS8 = 160nS.
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    val = ST7529_DATA_GPIO->IDR & ST7529_DATA_PINS;
    ST7529_rdHIGH();
    ST7529_csHIGH();
    ST7529_a0HIGH();
    return val;
}

/* Set the line address on the ST7529 */
static void ST7529_writeLineAddr( uint8_t startLine, uint8_t endLine )
{
    ST7529_writeCMD(LCD_LINE_ADDR_SET);
    ST7529_writeDATA(LINE_ADDR_SET_PB1_START_LINE_PUT(startLine + LCD_LINE_OFFSET));
    ST7529_writeDATA(LINE_ADDR_SET_PB2_END_LINE_PUT(endLine + LCD_LINE_OFFSET));
}

/* Set the column address on the ST7529 */
static void ST7529_writeColAddr( uint8_t startCol, uint8_t endCol )
{
    ST7529_writeCMD(LCD_COL_ADDR_SET);
    ST7529_writeDATA(COL_ADDR_SET_PB1_START_COL_PUT(startCol + LCD_COLUMN_OFFSET));
    ST7529_writeDATA(COL_ADDR_SET_PB2_END_COL_PUT(endCol + LCD_COLUMN_OFFSET));

}

/* call before streaming data */
static void ST7529_writeDATAStreamPrep(void)
{
    ST7529_a0HIGH();
}

/* set a contrast value into the controller */
void ST7529_writeContrast(uint16_t contrast)
{
    // Set the current contrast
    ST7529_writeCMD(LCD_CONTRAST_CONTROL);
    ST7529_writeDATA(contrast & 0b111111);
    ST7529_writeDATA(contrast >> 6);
}

#define EEPROM_DELAY (100000)

#if 0
static void dumb_usleep(unsigned int delusecs)
{
    int i;
    for (i = 0; i < 36 * delusecs; i++ );
}
#define usleep dumb_usleep
#endif

/* save the contrast adjustment to the controller */
void ST7529_saveContrastAdjToEEPROM(void)
{
    // Save contrast as default (to the EEPROM)

    /* EXT0 Mode */
    ST7529_writeCMD(LCD_EXT_SET_0);
    /* Display Off */
    ST7529_writeCMD(LCD_DISPLAY_OFF);
    /* Initial Code "improves the EEPROM internal ACK" */
    ST7529_writeCMD(LCD_INITIAL_CODE);
    ST7529_writeDATA(0x19);  /* Fixed Param */
    /* EXT1 Mode */
    ST7529_writeCMD(LCD_EXT_SET_1);
    /* EEPROM On */
    ST7529_writeCMD(LCD_CONTROL_EEPROM);
    ST7529_writeDATA(0x20); /* Write Mode */
    /*  delay */
    usleep(EEPROM_DELAY);
    /* Start the Write */
    ST7529_writeCMD(LCD_WRITE_EEPROM);
    /* delay */
    usleep(EEPROM_DELAY);
    /* Exit EEPROM Read */
    ST7529_writeCMD(LCD_CANCEL_EEPROM);

    /* Return To EXT0 Mode */
    ST7529_writeCMD(LCD_EXT_SET_0);

    /* Display On */
    ST7529_writeCMD(LCD_DISPLAY_ON);
}

/* load the default contrast adjustment (from the EEPROM) */
void ST7529_loadContrastAdjFromEEPROM(void)
{
    /* EXT0 Mode */
    ST7529_writeCMD(LCD_EXT_SET_0);
    /* Initial Code "improves the EEPROM internal ACK" */
    ST7529_writeCMD(LCD_INITIAL_CODE);
    ST7529_writeDATA(0x19);  /* Fixed Param */
    /* EXT1 Mode */
    ST7529_writeCMD(LCD_EXT_SET_1);
    /* EEPROM On */
    ST7529_writeCMD(LCD_CONTROL_EEPROM);
    ST7529_writeDATA(0x0); /* Read Mode */
    /* 100ms delay */
    usleep(EEPROM_DELAY);
    /* Start the Read */
    ST7529_writeCMD(LCD_READ_EEPROM);
    /* 100ms delay */
    usleep(EEPROM_DELAY);
    /* Exit EEPROM Read */
    ST7529_writeCMD(LCD_CANCEL_EEPROM);

    /* Return To EXT0 Mode */
    ST7529_writeCMD(LCD_EXT_SET_0);
}

#undef usleep

/* contrast up */
void ST7529_volumeUpContrast(void)
{
    ST7529_writeCMD(LCD_CONTRAST_INCREASE);
}

/* contrast down */
void ST7529_volumeDownContrast(void)
{
    ST7529_writeCMD(LCD_CONTRAST_DECREASE);
}

/* write a buffer to the controller */
void ST7529_bufferToLCD(uint8_t * bufferMem)
{
    int col, row;

    /* Write LCD lines 46 to 79 */
    ST7529_writeLineAddr(46, 79);
    ST7529_writeColAddr(3, 84);

    ST7529_writeCMD(LCD_MEM_WRITE);
    ST7529_writeDATAStreamPrep();

    for (row = 0; row <= 33; ++row) {
        /* Dummy writes for first 2 LCD pixels that aren't mapped */
        ST7529_writeDATA(0);
        ST7529_writeDATA(0);
        for (col = 0; col < LCD_USABLE_PIXELS_PER_ROW; ++col) {
            ST7529_writeDATA(*bufferMem++);
        }
    }

    /* Skip some unmapped lines */

    /* Write LCD lines 126 to 159 */
    ST7529_writeLineAddr(126, 159);
    ST7529_writeColAddr(3, 84);

    ST7529_writeCMD(LCD_MEM_WRITE);
    ST7529_writeDATAStreamPrep();

    for (row = 0; row <= 33; ++row) {
        /* Dummy writes for first 2 LCD pixels that aren't mapped */
        ST7529_writeDATA(0);
        ST7529_writeDATA(0);
        for (col = 0; col < LCD_USABLE_PIXELS_PER_ROW; ++col) {
            ST7529_writeDATA(*bufferMem++);
        }
    }
}
