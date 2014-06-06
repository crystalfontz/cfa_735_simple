#
# Description:
#
# Makefile for the cfa-735 simple user code.
#
# License:
#
# Copyright 2012 Crystalfontz America, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#TARGET=arm-none-eabi
TARGET=arm-linux-gnueabi
CC=$(TARGET)-gcc
OBJDUMP=$(TARGET)-objdump
OBJCOPY=$(TARGET)-objcopy

ST_SOURCES_CORE = STM32_USB-FS-Device_Lib/Libraries/CMSIS/CM3/CoreSupport/core_cm3.c
ST_SOURCES_DEVICE = STM32_USB-FS-Device_Lib/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c
ST_STARTUP = STM32_USB-FS-Device_Lib/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_hd.s
ST_SOURCES_PERIPH = STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/misc.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_cec.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c

ST_SOURCES_USB = STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/otgd_fs_pcd.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/usb_regs.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/otgd_fs_cal.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/usb_init.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/otgd_fs_dev.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/usb_mem.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/usb_core.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/usb_sil.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/otgd_fs_int.c \
	STM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/src/usb_int.c


ST_SOURCES = $(ST_SOURCES_CORE) $(ST_SOURCES_DEVICE) $(ST_SOURCES_PERIPH) $(ST_SOURCES_USB)
CF_SOURCES = main.c process_cmd.c start_screen.c lcd.c st7529_core.c systick.c keys.c leds.c ring_buffer.c uart.c 08x08fnt.c usb_desc.c usb_interrupt.c usb_istr.c usb_prop.c usb_pwr.c usb_pwr_modes.c usb_vcom.c
LDSCRIPT = linker_scripts/whole_chip.ld

INCLUDES = -ISTM32_USB-FS-Device_Lib/Libraries/CMSIS/CM3/CoreSupport \
	-ISTM32_USB-FS-Device_Lib/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
	-ISTM32_USB-FS-Device_Lib/Libraries/STM32F10x_StdPeriph_Driver/inc \
	-ISTM32_USB-FS-Device_Lib/Libraries/STM32_USB-FS-Device_Driver/inc \
	-Ipt-1.4 \
	-I.


# Build the code from the ST libraries and the Crystalfontz examples.
# The code is built for a ARM Cortex M3 (Thumb2), unoptimized with functions and data sectioned for unused section garbage collection at link time.
SOURCES = $(ST_SOURCES) $(CF_SOURCES)
OPTIM = -O0
CCFLAGS =  -fno-stack-protector -fdata-sections -ffunction-sections -Wall -D USE_CFA_735_V0_9 -D STM32F10X_HD -D HSE_VALUE=16000000 -D USE_STDPERIPH_DRIVER -mcpu=cortex-m3 -mthumb -Wcast-align $(OPTIM) -fomit-frame-pointer -ggdb $(INCLUDES)
LINKFLAGS = -T$(LDSCRIPT) -Xlinker --gc-sections -Xlinker -M -Xlinker -Map=$@.map -nostdlib $(CCFLAGS)

cfa735.elf: $(SOURCES:.c=.o) $(ST_STARTUP)
	$(CC) $(LINKFLAGS) -o $@ $^

cfa735.elf.asm: cfa735.elf
	$(OBJDUMP) -D $^ > $@

REMOVE_SECTION=.note.gnu.build-id
cfa735.elf.hex: cfa735.elf
	$(OBJCOPY)  -O srec --strip-debug $(foreach rs, $(REMOVE_SECTION), --remove-section $(rs)) $^  $@

cfa735.elf.sections: cfa735.elf
	$(OBJDUMP)  -h   $^  > $@

all: cfa735.elf cfa735.elf.asm cfa735.elf.hex cfa735.elf.sections 
	

clean:
	rm -v $(SOURCES:.c=.o) $(SOURCES:.c=.P) cfa735.elf cfa735.elf.*


%.o : %.c
	$(CC) $(CCFLAGS) -MD -c -o $@ $<
	@cp $*.d $*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
		rm -f $*.d

-include $(SOURCES:.c=.P)

