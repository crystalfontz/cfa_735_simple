/*
 * Description:
 *
 * High level USB functions for a Virtual COM port
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
 * USB Endpoint 1 is used to send data to the host (an IN endpoint)
 *
 * USB Endpoint 3 is used to receive data from the host (an OUT endpoint)
 */

#include "usb_vcom.h"
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include <string.h>
#include "ring_buffer.h"

void EP1_IN_Callback(void);
void EP3_OUT_Callback(void);

static struct RingBuffer tx_ring;
static struct RingBuffer rx_ring;
static int write_ready = 1;

/* number of characters for the configurable descriptors (actual size is 2*(n+1) for unicode) */
#define STRING_DESCRIPTOR_MAX_CHARS 20

/* init the STM32 clocks for the USB controller */
static void initUSBClocks()
{
#ifdef STM32F10X_CL
    /* Select USBCLK source */
    RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

    /* Enable the USB clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
#else
    /* Select USBCLK source */
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

    /* Enable the USB clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#endif /* STM32F10X_CL */
}

/* init the interrupt controller to handle USB interrupts */
static void initUSBInterrupts()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Set the NVIC to accept interrupt for the USB device.
     Our priority structure defines this as 0:1 priority so
     only 0:0 priority interrupts can preempt it. */

#ifdef STM32F10X_CL
    /* Enable the USB Interrupts */
    NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
#else
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
#endif /* STM32F10X_CL */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* This takes a C style string and converts it to a UNICODE string for USB descriptor strings */
static void Update_String_Desciptor_CString(ONE_DESCRIPTOR* desc, const char* c_string, uint8_t unicode_buffer[STRING_DESCRIPTOR_MAX_CHARS*2+2])
{
    // take the c string and accont for a unicode size plus add 2 bytes for size and type
    unsigned int c_string_len = 0;
    if (c_string)
    {
        const char* c = c_string;
        while (*c && c_string_len < STRING_DESCRIPTOR_MAX_CHARS)
        {
            ++c_string_len;
            ++c;
        }
    }

    desc->Descriptor_Size = c_string_len*2 + 2;
    desc->Descriptor = unicode_buffer;
    desc->Descriptor[0] = desc->Descriptor_Size; /* bLength */
    desc->Descriptor[1] = USB_STRING_DESCRIPTOR_TYPE; /* bDescriptorType */
    unsigned int i = 0;
    while(i < c_string_len)
    {
        desc->Descriptor[i*2+2] = c_string[i];
        desc->Descriptor[i*2+2+1] = '\0';
        i++;
    }
}

/* Set the Vendor String into the USB descriptor */
void USB_VCOMSetVendorString(const char* s)
{
    static uint8_t desc_buffer[STRING_DESCRIPTOR_MAX_CHARS*2+2];
    Update_String_Desciptor_CString(&String_Descriptor[1], s, desc_buffer);
}

/* Set the Product String into the USB descriptor */
void USB_VCOMSetProductString(const char* s)
{
    static uint8_t desc_buffer[STRING_DESCRIPTOR_MAX_CHARS*2+2];
    Update_String_Desciptor_CString(&String_Descriptor[2], s, desc_buffer);
}

/* Set the Serial Number String into the USB descriptor */
void USB_VCOMSetSerialNumberString(const char* s)
{
    static uint8_t desc_buffer[STRING_DESCRIPTOR_MAX_CHARS*2+2];
    Update_String_Desciptor_CString(&String_Descriptor[3], s, desc_buffer);
}

/* Init the Virtual COM port USB device */
void USB_VCOMinit()
{
    // Set up clocks
    initUSBClocks();

    // Set up interrupts
    initUSBInterrupts();

    // Set up USN controller
    USB_Init();
}

/* read data from endpoint's ring */
unsigned int USB_VCOMread(unsigned int size, void* buffer)
{
    return GetDataFromRing(&rx_ring, size, (uint8_t*)buffer);
}

/* read a full block or less in EndPoint3 every callback */
void EP3_OUT_Callback(void)
{
    static uint8_t buffer[VIRTUAL_COM_PORT_DATA_SIZE];
    unsigned int bytes = USB_SIL_Read(EP3_OUT, (uint8_t*)buffer);
    PutDataInRing(&rx_ring, bytes, buffer);
#ifndef STM32F10X_CL
    /* Enable the receive of data on EP3 */
    SetEPRxValid(ENDP3);
#endif /* STM32F10X_CL */
}

/* write some amount of data in blocks out the end point */
void USB_VCOMwrite(unsigned int size, void* buffer)
{
    PutDataInRing(&tx_ring, size, (uint8_t*) buffer);
    if (write_ready)
    {
        write_ready = 0;
        EP1_IN_Callback();
    }
}

/* write a full block or less out EndPoint1 every callback */
void EP1_IN_Callback(void)
{
    static uint8_t buffer[VIRTUAL_COM_PORT_DATA_SIZE];
    unsigned int bytes = GetDataFromRing(&tx_ring, VIRTUAL_COM_PORT_DATA_SIZE, buffer);
    if (bytes)
    {
        USB_SIL_Write(EP1_IN, buffer, bytes);
#ifndef STM32F10X_CL
        SetEPTxValid(ENDP1);
#endif /* STM32F10X_CL */
    }
    else
    {
        write_ready = 1;
    }
}
