/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Description of the binary directory.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Description
===================
This directory contains a set of sources files that build the application to be
loaded into Flash memory using device firmware upgrade (DFU).

To build such application, some special configuration has to be preformed: 
1. Set the program load address at 0x08003000, using your toolchain linker file
2. Relocate the vector table at address 0x08003000, using the "NVIC_SetVectorTable"
   function.
 
The SysTick example provided within the STM32F10xxx and STM32L152xxx Standard 
Peripheral examples is used as illustration. 
   
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to 72 MHz for STM32F10xxx devices and to 32MHz for 
STM32L15xxx devices, the SysTick is clocked by the AHB clock (HCLK)

A "Delay" function is implemented based on the SysTick end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


Directory contents
==================

  + binary_template\EWARM: This folder contains a preconfigured project file 
                             that produces a binary image of SysTick example to 
                             be loaded with DFU.

  + binary_template\RIDE: This folder contains a preconfigured project file 
                          that produces a binary image of SysTick example to be 
                          loaded with DFU.

  + binary_template\MDK-ARM: This folder contains a preconfigured project file 
                           that produces a binary image of SysTick example to be 
                           loaded with DFU.

  + binary_template\HiTOP: This folder contains a preconfigured project file 
                           that produces a binary image of SysTick example to be 
                           loaded with DFU.

 + \TrueSTUDIO: This folder contains a preconfigured project file 
                that produces a binary image of SysTick example to be 
                loaded with DFU.                                      
                  
  + binary_template\inc: contains the binary_template firmware header files 
    - stm32f10x_conf.h    Library Configuration file for STM32F10xxx devices
    - stm32f10x_it.h      Header for stm32f10x_it.c
    - stm32l15x_conf.h    Library Configuration file for STM32L15xxx devices
    - stm32l15x_it.h      Header for stm32l15x_it.c
    - main.h              Header for main.c

  + binary_template\src: contains the binary_template firmware source files 
    - main.c              Main program
    - stm32f10x_it.c      Interrupt handlers for STM32F10xxx devices
    - stm32f10x_it.c      Interrupt handlers for STM32L15xxx devices

Hardware environment
====================
This example runs on STMicroelectronics STM3210B-EVAL, STM3210C-EVAL, STM3210E-EVAL 
and STM32L152-EVAL evaluation boards and can be easily tailored to any other hardware.
The selection of the evaluation board is done through the define into the project
configuration. 


 + STM3210B-EVAL 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
 
 + STM3210C-EVAL
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PD.07, PD.13, PD.03
      and PD.04 pins

 + STM3210E-EVAL
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PF.06, PF.07, PF.08
      and PF.09 pins

 + STM32L152-EVAL
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PD.00, PD.01, PD.02
      and PC.12 pins
                    
   
How to use it
=============
In order to load the SysTick example with the DFU, you must do the following:
 
  + EWARM
    - Open the SysTick.eww workspace
    - In the workspace toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210C-EVAL: to configure the project for STM32 Connectivity-Line devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 Medium-Density Low-Power devices
   - Rebuild all files: Project->Rebuild all
   - Go to "Utilities\Binary" and run "hextobin.bat"
   - A binary file "SysTick.bin" will be generated under "STM3210E-EVAL\Exe" or
     "STM3210C-EVAL\Exe" or "STM3210B-EVAL\Exe" or "STM32L152-EVAL\Exe" folder depending
     on configuration you selected. 

  + MDK-ARM
    - Open the SysTick.Uv2 project
    - In the build toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210C-EVAL: to configure the project for STM32 Connectivity-Line devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Low Power Medium-density devices
    - Rebuild all files: Project->Rebuild all target files
    - Go to "Utilities\Binary" and run "axftobin.bat"
    - A binary file "STM3210B-EVAL_SysTick.bin" or "STM3210C-EVAL_SysTick.bin" or 
      "STM3210E-EVAL_SysTick.bin"  or "STM32L152-EVAL_SysTick.bin" will be generated
       under "\Obj" folder.
       
 + RIDE
    - Open the SysTick.rprj project
    - In the configuration toolbar(Project->properties) select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210C-EVAL: to configure the project for STM32 Connectivity-Line devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
    - Rebuild all files: Project->build project
    - Go to "Utilities\Binary" and run "hextobin.bat"
    - A binary file "SysTick.bin" will be generated under "\STM3210E_EVAL" 
      or "\STM3210C_EVAL" or "\STM3210B_EVAL" or "\STM32L152_EVAL" folder depending
      on configuration you selected.
      
  + HiTOP
    - Open the HiTOP toolchain, a "using projects in HiTOP" window appears.
    - Select open an existing project.
    - Browse to open the SysTick.htp:
        - under STM32F10B_EVAL directory: to select the project for STM32 Medium-density devices.
        - under STM32F10C_EVAL directory: to select the project for STM32 Connectivity-Line devices.
        - under STM32F10E_EVAL directory: to select the project for STM32 High-density devices
        - under STM32F10E_EVAL_XL directory: to select the project for STM32 XL-density devices
        - under STM32L152_EVAL directory: to select the project for STM32 Medium-Density Low-Power devices
    - "Download application" window is displayed, click "cancel".
    - Rebuild all files: Project->Rebuild all
    - Go to "Utilities\Binary" directory and run "hex2bin.bat"
    - A binary file "Project.bin" will be generated in the project folder
    

 + TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project:  
        - STM3210B-EVAL: to load the project for STM32 Medium-density devices
        - STM3210C-EVAL: to load the project for STM32 Connectivity line devices
        - STM3210E-EVAL: to load the project for STM32 High-density devices
        - STM3210E_EVAL_XL: to load the project for STM32 XL-density devices
        - STM32L152_EVAL: to load the project for STM32 Medium-Density Low-Power devices
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click 
      on Project->build project menu.
    - Go to "Utilities\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM32F10X_XL_SysTick.bin", "STM3210C-EVAL_SysTick.bin", 
      "STM3210B-EVAL_SysTick.bin", "STM3210E-EVAL_SysTick.bin", 
      "STM32L152-EVAL_SysTick.bin" will be generated under "\Debug" folder

 + The generated .bin file should be converted to the DFU format using the “DFU File
   Manager Tool” included in the “DfuSe” PC software install. For more details on
   how to convert a .bin file to DFU format please refer to the UM0412 user manual
   “Getting started with DfuSe USB device firmware upgrade STMicroelectronics extension”
   available from the STMicroelectronics microcontroller website www.st.com.

NOTE:
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers. 
 - Medium-density Low-Power devices are STM32L15xx microcontrollers where the flash
   memory density ranges between 64 and 128 Kbytes

******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE******
