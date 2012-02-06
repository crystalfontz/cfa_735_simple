/*
 * Description:
 *
 * Function header for buffered Serial UART I/O from the H1 connector
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
#ifndef __UART_H__
#define __UART_H__

/* uart config return codes */
#define UARTRetOK (0)
#define UARTRetBadSpeed (3)
#define UARTRetBadParity (4)

/* uart parity settings, all communication is 8-bit */
#define UARTParity_No (0)
#define UARTParity_Odd (1)
#define UARTParity_Even (2)

/* init the uart */
int UARTinit();
/* enable the uart with the given settings */
int UARTenable(unsigned int baudrate, int parity);
/* disable the uart */
void UARTdisable();
/* read data from the uart buffer in to the provided buffer of at max size bytes */
unsigned int UARTread(unsigned int size, void* buffer);
/* write the data in buffer of the given size into the uart buffer to be transmitted */
void UARTwrite(unsigned int size, void* buffer);

#endif /* __UART_H__ */
