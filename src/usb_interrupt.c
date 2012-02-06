/*
 * Description:
 *
 * USB Interrupt handler to call the specific handler for a STM32F10x chip
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
 * This is the NVIC handler that calls the USB interrupt hander
 */

#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_istr.h"

#ifndef STM32F10X_CL
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    USB_Istr();
}
#else /* STM32F10X_CL */
void OTG_FS_IRQHandler(void)
{
    STM32_PCD_OTG_ISR_Handler();
}
#endif /* STM32F10X_CL */
