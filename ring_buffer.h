/*
 * Description:
 *
 * Functions for fixed size ring buffer.
 *
 * The Get and Put are interrupt safe.
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
#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <stdint.h>

/* usable size of the ring buffer */
#define RING_SIZE 128
/* internal size of the ring buffer */
#define RING_STORE_SIZE (RING_SIZE + 1)

/* ring buffer structure */
struct RingBuffer
{
    uint8_t buffer[RING_STORE_SIZE];
    unsigned int head;
    unsigned int tail;
};

/* init a ring buffer by passing in a pointer to a ring buffer structure */
void InitRing(struct RingBuffer* ring);
/* retrieve get at most size bytes from a ring buffer and put them into the passed in buffer pointer, return the bytes retrieved */
unsigned int GetDataFromRing(struct RingBuffer* ring, unsigned int size, uint8_t* buffer);
/* put size bytes from buffer into the ring buffer */
void PutDataInRing(struct RingBuffer* ring, unsigned int size, uint8_t* buffer);

#endif /* __RING_BUFFER_H__ */

