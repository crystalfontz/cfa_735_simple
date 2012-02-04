/*
 * Description:
 *
 * UART functions for buffered I/O
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
#include "ring_buffer.h"
#include "platform_config.h"
#include "stm32f10x.h"
#include "uart.h"

static struct RingBuffer tx_ring;
static struct RingBuffer rx_ring;


/* Configure the Pins used by this UART */
static void initPins(int enable)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    /* Enable the Port for the corresponding GPIOs */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* Configure USART Rx as input pulled-up */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(H1UART_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    if (enable)
    {
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    }
    GPIO_Init(H1UART_GPIO_PORT, &GPIO_InitStructure);

    /* Enable the USART Pins Software Remapping */
    if (H1UART_REMAP) {
        GPIO_PinRemapConfig(H1UART_REMAP, enable?ENABLE:DISABLE);
    }
}

/* Clock the UART Peripheral */
static void initClocks()
{
    /* Clock the UART */
    H1UART_CLK_CMD(H1UART_CLK, ENABLE);

}

/* Set up the UARTs interrupt */
static void initInterruptController()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USART Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = H1UART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* Call the applications call back on interrupt */
void H1UART_HANDLER(void)
{
    if (USART_GetFlagStatus(H1UART, USART_FLAG_TXE))
    {
        uint8_t byte;
        if (GetDataFromRing(&tx_ring, 1, &byte))
        {
            USART_SendData(H1UART, byte);
        }
        else
        {
            USART_ITConfig(H1UART, USART_IT_TXE, DISABLE);
        }
    }

    if (USART_GetFlagStatus(H1UART, USART_FLAG_RXNE))
    {
        uint8_t byte = USART_ReceiveData(H1UART);
        PutDataInRing(&rx_ring, 1, &byte);
    }
}

/* Enable and config the UART port */
int UARTenable(unsigned int baudrate, int parity)
{
    if (IS_USART_BAUDRATE(baudrate)) {
        USART_InitTypeDef USART_InitStructure;
        USART_StructInit(&USART_InitStructure);

        USART_InitStructure.USART_BaudRate = baudrate;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        /* always use 8-bit data */
        switch (parity) {
        case UARTParity_No:
            /* 8 bits and no parity bit */
            USART_InitStructure.USART_WordLength = USART_WordLength_8b;
            USART_InitStructure.USART_Parity = USART_Parity_No;
            break;
        case UARTParity_Even:
            /* 8 bits and a parity bit */
            USART_InitStructure.USART_WordLength = USART_WordLength_9b;
            USART_InitStructure.USART_Parity = USART_Parity_Even;
            break;
        case UARTParity_Odd:
            /* 8 bits and a parity bit */
            USART_InitStructure.USART_WordLength = USART_WordLength_9b;
            USART_InitStructure.USART_Parity = USART_Parity_Odd;
            break;
        default:
            return UARTRetBadParity;
        }

        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

        /* Configure USART */
        USART_DeInit(H1UART);
        USART_Init(H1UART, &USART_InitStructure);

        /* Get interrupts on Recieve Not Empty, enable TXE as needed later */
        USART_ITConfig(H1UART, USART_IT_RXNE, ENABLE);

        /* Go */
        USART_Cmd(H1UART, ENABLE);

        /* wait for tx ready */
        while(USART_GetFlagStatus(H1UART, USART_FLAG_TC) == RESET);

        /* Set up pins */
        initPins(1);

        return UARTRetOK;
    }
    else {
        return UARTRetBadSpeed;
    }
}

/* Disable the UART */
void UARTdisable()
{
    /* Float pins */
    initPins(0);

    /* DeInit */
    USART_DeInit(H1UART);
}

/* Initialize this UARTs Pins and clocks without enabling the port */
int UARTinit()
{
    /* Set up Rings */
    InitRing(&tx_ring);
    InitRing(&rx_ring);

    /* Set up clocks */
    initClocks();

    /* Set up interrupts */
    initInterruptController();

    return UARTRetOK;
}


/* read as much data as available data from the ring into the provided buffer */
unsigned int UARTread(unsigned int size, void* buffer)
{
    return GetDataFromRing(&rx_ring, size, (uint8_t*)buffer);
}

/* write the data in buffer into the ring and enable the interrupt to transfer it */
void UARTwrite(unsigned int size, void* buffer)
{
    PutDataInRing(&tx_ring, size, (uint8_t*) buffer);
    USART_ITConfig(H1UART, USART_IT_TXE, ENABLE);
}

