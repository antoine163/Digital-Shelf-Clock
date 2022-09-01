/***
 * The MIT License (MIT)
 * 
 * Copyright (c) 2022 antoine163
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Include ---------------------------------------------------------------------
#include "mp/utils/fifo.h"

// Std
#include <string.h>

// Implemented functions -------------------------------------------------------
void mp_fifo_init(mp_fifo_t * fifo, uint8_t * storageBuffer, unsigned int size)
{
    fifo->buffer = storageBuffer;
    fifo->bufferSize = size;
    fifo->bufferEnd = storageBuffer + size;
    mp_fifo_clean(fifo);
}

void mp_fifo_clean(mp_fifo_t * fifo)
{
    fifo->writeTo = fifo->buffer;
    fifo->readFrom = fifo->buffer;
    fifo->used = 0;
}

int mp_fifo_push(mp_fifo_t * fifo, uint8_t const * buf, unsigned int nByte)
{
    // Get the number of bytes can be copy in to storage area.
    unsigned int unusedBytes = mp_fifo_unused(fifo);
    if ( nByte > unusedBytes )
        nByte = unusedBytes;
    
    // Compute the number of bytes can be copy between 'writeTo' and the end of
    // storage area.
    unsigned int unusedByteToEnd = fifo->bufferEnd - fifo->writeTo;
    
    // There is enough free space to end of storage area ?
    if (nByte < unusedByteToEnd) // Yes (this is majority of case)
    {
        memcpy(fifo->writeTo, buf, nByte);
        fifo->writeTo += nByte;
    }
    else // No
    {
        // Copy the fist part of buf to the end of storage area.
        memcpy(fifo->writeTo, buf, unusedByteToEnd);
        
        buf += unusedByteToEnd;
        unsigned int nByteFromStart = nByte - unusedByteToEnd;
        
        // Copy the last part of buf to the start of storage area.
        memcpy(fifo->buffer, buf, nByteFromStart);
        
        fifo->writeTo = fifo->buffer + nByteFromStart;
    }
    
    fifo->used += nByte;
    return nByte;
}

int mp_fifo_pop(mp_fifo_t * fifo, uint8_t * buf, unsigned int nByte)
{
    // Get the number of bytes can be copy in to buf.
    unsigned int usedBytes = mp_fifo_used(fifo);
    if (nByte > usedBytes)
        nByte = usedBytes;
    
    // Compute the number of bytes can be copy between 'readFrom' and the end of
    // storage area.
    unsigned int usedByteToEnd = fifo->bufferEnd - fifo->readFrom;
    
    // All data to read are in the end of storage area ?
    if (nByte < usedByteToEnd)  // Yes ( this is majority of case )
    {
        memcpy(buf, fifo->readFrom, nByte);
        fifo->readFrom += nByte;
    }
    else // No
    {
        // Copy the end of storage area to the first part of buf.
        memcpy(buf, fifo->readFrom, usedByteToEnd);
        
        buf += usedByteToEnd;
        unsigned int nByteFromStart = nByte - usedByteToEnd;
        
        // Copy the start of storage area to the last part of buf. 
        memcpy(buf, fifo->buffer, nByteFromStart);
        
        fifo->readFrom = fifo->buffer + nByteFromStart;
        fifo->used -= nByte;
    }
    
    fifo->used -= nByte;
    return nByte;
}

int mp_fifo_pushByte(mp_fifo_t * fifo, uint8_t byte)
{
    if (!mp_fifo_isFull(fifo))
    {
        MP_FIFO_PUSH_BYTE(fifo, byte);
        return 1;
    }
    
    return 0;
}

int mp_fifo_popByte(mp_fifo_t * fifo, uint8_t * byte)
{
    if (!mp_fifo_isEmpty(fifo))
    {
        MP_FIFO_POP_BYTE(fifo, byte);
        return 1;
    }
    
    return 0;
}
