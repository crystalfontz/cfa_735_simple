/*
 * Description:
 *
 * Function header for buffered USB I/O
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
#ifndef __USB_VCOM_H__
#define __USB_VCOM_H__

/* These can be used to set some of the USB descriptors at run-time, before init */
/* Note that the c strings are translated in these calls into Unicode strings */
void USB_VCOMSetVendorString(const char* s);
void USB_VCOMSetProductString(const char* s);
void USB_VCOMSetSerialNumberString(const char* s);

/* initialize the USB VCOM */
void USB_VCOMinit();

/* read data from the USB buffer in to the provided buffer of at max size bytes */
unsigned int USB_VCOMread(unsigned int size, void* buffer);
/* write the data in buffer of the given size into the USB buffer to be transmitted */
void USB_VCOMwrite(unsigned int size, void* buffer);

#endif
