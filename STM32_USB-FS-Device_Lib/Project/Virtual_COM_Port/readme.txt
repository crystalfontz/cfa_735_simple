/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Description of the USB Virtual_COM_Port Demo.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Example description
===================
In modern PCs, USB is the standard communication port for almost all peripherals. 
However many industrial software applications still use the classic COM Port (UART).
The Virtual COM Port Demo provides a simple solution to bypass this problem; it
uses the USB-FS-Device as a COM port by affecting the legacy PC application designed for
COM Port communication.
This Virtual COM Port Demo provides the firmware examples for the STM32F10xxx
family.

Note that with this demo only the following configuration are supported:
-> 7bit data with parity (Even or Odd).
-> 8bit data with and without parity.

This demo is using two different methods for the IN and OUT transfers in order 
to manage the data rate difference between USB and USART buses:

 - OUT transfers (from Host to Device):
     When a packet is received from the host on the OUT pipe (EP3), the Endpoint
     callback function is called when the transfer is complete and all the received
     data are then sent through USART peripheral in polling mode. This allows all
     incoming OUT packets to be NAKed till the current packet is completely transferred
     through the USART interface.
 
 - IN transfers (from Device to Host):
     For IN data, a large circular buffer is used. USART and USB respectively write
     and read to/from this buffer independently.
     USART RXNE interrupt is used to write data into the buffer. This interrupt 
     has the highest priority, which allows to avoid overrun and data loss conditions.
     USB IN endpoint (EP1) is written with the received data into the SOF interrupt 
     callback. 
     Into this callback, a frame counter counts the number of passed frames since 
     the last time IN endpoint was written. This allows to write IN endpoint at
     separated frames when enough data are present in the data buffer. 
     To modify the number of frame between IN write operations, modify the value 
     of the define "VCOMPORT_IN_FRAME_INTERVAL" in "usb_endp.c" file.
     To allow high data rate performance, SOF interrupt is managed with highest 
     priority (thus SOF interrupt is checked before all other interrupts).     


More details about this Demo implementation is given in the User manual 
"UM0424 STM32F10xxx USB development kit", available for download from the ST
microcontrollers website: www.st.com/stm32


Directory contents
==================
 + \inc: contains the Demo firmware header files
 + \EWARM: contains preconfigured projects for EWARM toolchain
 + \RIDE: contains preconfigured projects for RIDE toolchain
 + \MDK-ARM: contains preconfigured projects for MDK-ARM toolchain
 + \HiTOP: contains preconfigured projects for HiTOP toolchain
 + \TrueSTUDIO: contains preconfigured projects for TrueSTUDIO toolchain          
 + \src: contains the Demo firmware source files

Hardware environment
====================
This example runs on STMicroelectronics STM3210B-EVAL, STM3210C-EVAL, STM3210E-EVAL 
and STM32L152-EVAL evaluation boards and can be easily tailored to any other hardware.
To select the STMicroelectronics evaluation board used to run the example, uncomment
the corresponding line in platform_config.h file.

  - STM3210B-EVAL Set-up 
     - Jumper JP1 (USB disconnect) should be connected in position 2-3.
     - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN6 and Host (PC) serial port.

  - STM3210E-EVAL Set-up 
     - Jumper JP14 (USB disconnect) should be connected in position 2-3.
     - Connect a null-modem female/female RS232 cable between the DB9 connector
      CN12 and Host (PC) serial port. 

  - STM3210C-EVAL Set-up 
     - Jumper JP17 (I2C_SCK) should be connected.
     - Jumpers JP19 and JP18 should be not connected.
     - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN6 and Host (PC) serial port.  

  - STM32L152-EVAL Set-up 
     - Jumper JP19 should be not connected.
     - Jumper JP5 should be connected in 2-3 position (RS232)
     - Connect a null-modem female/female RS232 cable between the DB9 connector
      CN2 and Host (PC) serial port. 

    
How to use it
=============
 + EWARM
    - Open the VirtualCOMPort.eww workspace.
    - In the workspace toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210C-EVAL: to configure the project for STM32 Connectivity-Line devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the VirtualCOMPort.Uv2 project
    - In the build toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210C-EVAL: to configure the project for STM32 Connectivity-Line devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5) 

 + RIDE
    - Open the VirtualCOMPort.rprj project.
    - In the configuration toolbar(Project->properties) select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210C-EVAL: to configure the project for STM32 Connectivity-Line devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
        - STM3210E-EVAL_XL: to configure the project for STM32 XL-density devices
        - STM32L152-EVAL: to configure the project for STM32 Medium-Density Low-Power devices
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)

 + HiTOP
    - Open the HiTOP toolchain, a "using projects in HiTOP" window appears.
    - Select open an existing project.
    - Browse to open the VirtualCOMPort.htp:
        - under STM32F10B_EVAL directory: to select the project for STM32 Medium-density devices.
        - under STM32F10C_EVAL directory: to select the project for STM32 Connectivity-Line devices.
        - under STM32F10E_EVAL directory: to select the project for STM32 High-density devices
        - under STM32F10E_EVAL_XL directory: to select the project for STM32 XL-density devices
        - under STM32L152_EVAL directory: to select the project for STM32 Medium-Density Low-Power devices
    - "Download application" window is displayed, click "cancel".
    - Rebuild all files: Project->Rebuild all
    - Click on ok in the "download project" window.
    - Run program: Debug->Go(F5).  

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
    - Under Windows->Preferences->General->Workspace->Linked Resources, add 
      a variable path named "CurPath" which points to the folder containing
      "Libraries", "Project" and "Utilities" folders.
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

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
