/*
 * Description:
 *
 * Implemenation of a simple ring buffer
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
#include "stm32f10x.h"
#include <string.h>

/* return how many bytes are used in the ring buffer */
static unsigned int iGetRemaining(const struct RingBuffer* ring)
{
    unsigned int avail;

    if (ring->tail >= ring->head)
        avail = ring->tail - ring->head;
    else
        avail = RING_STORE_SIZE - ring->head + ring->tail;

    return avail;
}

/* get a chunk of data before or after a boundary */
static unsigned int iGetDataFromRing(struct RingBuffer* ring, unsigned int size, uint8_t* buffer)
{
    unsigned int avail;
    if (ring->tail >= ring->head)
        avail = ring->tail - ring->head;
    else
        avail = RING_STORE_SIZE - ring->head;

    if (size > avail)
        size = avail;
    memcpy(buffer, &(ring->buffer[ring->head]), size);
    ring->head += size;
    if (ring->head >= RING_STORE_SIZE)
        ring->head = 0;
    return size;
}

/* get a chunk of data, wrapping if needed */
static unsigned int unsafeGetDataFromRing(struct RingBuffer* ring, unsigned int size, uint8_t* buffer)
{
    unsigned int bytes;
    unsigned int avail = iGetRemaining(ring);

    if (size > avail)
        size = avail;

    avail = size;

    /* first get */
    bytes = iGetDataFromRing(ring, avail, buffer);
    if (bytes != avail)
    {
        /* second get if wrapping was needed */
        buffer += bytes;
        avail -= bytes;
        avail -= iGetDataFromRing(ring, avail, buffer);
        assert_param(avail == 0);
    }

    return size;
}

/* put data into the ring */
static void unsafePutDataInRing(struct RingBuffer* ring, unsigned int size, uint8_t* buffer)
{
    unsigned int unused = RING_SIZE - iGetRemaining(ring);
    int overrun = unused < size;

    if (size >= RING_SIZE)
    {
        /* special case of being bigger than the buffer */
        unsigned int offset = size - RING_SIZE;
        memcpy(ring->buffer, buffer + offset, RING_SIZE);
        ring->head = 0;
        ring->tail = RING_SIZE;
    }
    else
    {
        unsigned int end = RING_STORE_SIZE - ring->tail;
        if (size < end)
        {
            memcpy(&(ring->buffer[ring->tail]), buffer, size);
            ring->tail = ring->tail + size;
        }
        else
        {
            memcpy(&(ring->buffer[ring->tail]), buffer, end);
            ring->tail = 0;
            memcpy(&(ring->buffer[0]), buffer+end, size-end);
            ring->tail = size - end;
        }

        if (overrun)
        {
            ring->head = ring->tail + 1;
            if (ring->head >= RING_STORE_SIZE)
                ring->head = 0;
        }
    }
}

/* init the ring */
void InitRing(struct RingBuffer* ring)
{
    ring->head = 0;
    ring->tail = 0;
}

/* safely get data from the ring buffer */
unsigned int GetDataFromRing(struct RingBuffer* ring, unsigned int size, uint8_t* buffer)
{
    unsigned int ret;
    __disable_irq();
    ret = unsafeGetDataFromRing(ring, size, buffer);
    __enable_irq();

    return ret;
}

/* safely put data into a ring buffer */
void PutDataInRing(struct RingBuffer* ring, unsigned int size, uint8_t* buffer)
{
    __disable_irq();
    unsafePutDataInRing(ring, size, buffer);
    __enable_irq();
}

