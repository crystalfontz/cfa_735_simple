/*
 * Description:
 *
 * USB Interrupt State function Header, functions defined by usb_conf.h
 *
 * Portions from the STM32F10x USB Full Speed Device Library
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
 * This file defines the callbacks and interrupts for the STM32 USB infrastructure
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_ISTR_H
#define __USB_ISTR_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifndef STM32F10X_CL
 void USB_Istr(void);
#else /* STM32F10X_CL */
 u32 STM32_PCD_OTG_ISR_Handler(void);
#endif /* STM32F10X_CL */

/* function prototypes Automatically built defining related macros */

void EP1_IN_Callback(void);
void EP2_IN_Callback(void);
void EP3_IN_Callback(void);
void EP4_IN_Callback(void);
void EP5_IN_Callback(void);
void EP6_IN_Callback(void);
void EP7_IN_Callback(void);

void EP1_OUT_Callback(void);
void EP2_OUT_Callback(void);
void EP3_OUT_Callback(void);
void EP4_OUT_Callback(void);
void EP5_OUT_Callback(void);
void EP6_OUT_Callback(void);
void EP7_OUT_Callback(void);

#ifndef STM32F10X_CL

#ifdef CTR_CALLBACK
void CTR_Callback(void);
#endif

#ifdef DOVR_CALLBACK
void DOVR_Callback(void);
#endif

#ifdef ERR_CALLBACK
void ERR_Callback(void);
#endif

#ifdef WKUP_CALLBACK
void WKUP_Callback(void);
#endif

#ifdef SUSP_CALLBACK
void SUSP_Callback(void);
#endif

#ifdef RESET_CALLBACK
void RESET_Callback(void);
#endif

#ifdef SOF_CALLBACK
void SOF_Callback(void);
#endif

#ifdef ESOF_CALLBACK
void ESOF_Callback(void);
#endif

#else /* STM32F10X_CL */

/* Interrupt subroutines user callbacks prototypes.
   These callbacks are called into the respective interrupt sunroutine functinos
   and can be tailored for various user application purposes.
     Note: Make sure that the correspondant interrupt is enabled through the 
     definition in usb_conf.h file */ 
void INTR_MODEMISMATCH_Callback(void);
void INTR_SOFINTR_Callback(void);
void INTR_RXSTSQLVL_Callback(void);
void INTR_NPTXFEMPTY_Callback(void);
void INTR_GINNAKEFF_Callback(void);
void INTR_GOUTNAKEFF_Callback(void);
void INTR_ERLYSUSPEND_Callback(void);
void INTR_USBSUSPEND_Callback(void);
void INTR_USBRESET_Callback(void);
void INTR_ENUMDONE_Callback(void);
void INTR_ISOOUTDROP_Callback(void);
void INTR_EOPFRAME_Callback(void);
void INTR_EPMISMATCH_Callback(void);
void INTR_INEPINTR_Callback(void);
void INTR_OUTEPINTR_Callback(void);
void INTR_INCOMPLISOIN_Callback(void);
void INTR_INCOMPLISOOUT_Callback(void);
void INTR_WKUPINTR_Callback(void);

/* Isochronous data update */
void INTR_RXSTSQLVL_ISODU_Callback(void); 

#endif /* STM32F10X_CL */


#endif /*__USB_ISTR_H*/
