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

#ifndef MP_TICK_H
#define MP_TICK_H

// Include ---------------------------------------------------------------------
#include "mp_tick_port.h"

// Define macro ----------------------------------------------------------------

/**
 * @brief Max value of @ref mp_tick_t type.
 * 
 * This value can be use for infinite timeout.
 */
#define MP_TICK_MAX     MP_TICK_PORT_MAX

/**
 * @brief Convert milliseconds to tick.
 * 
 * @param ms Value in milliseconds to convert.
 * @return The number of tick in @p ms.
 */
#define MP_TICK_FROM_MS(ms)     MP_TICK_PORT_FROM_MS(ms)

/**
 * @brief Convert tick to milliseconds.
 * 
 * @param tick The value in tick to convert.
 * @return The Number of milliseconds from @p tick.
 */
#define MP_TICK_TO_MS(tick)     MP_TICK_PORT_TO_MS(tick)

// Public static 'virtual' functions -------------------------------------------
static inline int mp_tick_init()
{
    return mp_tick_port_init();
}

static inline int mp_tick_deinit()
{
    return mp_tick_port_deinit();
}

static inline mp_tick_t mp_tick_get()
{
    return mp_tick_port_get();
}

static inline mp_tick_t mp_tick_getFromIsr()
{
    return mp_tick_port_getFromIsr();
}

static inline void mp_tick_delay(mp_tick_t delay)
{
    return mp_tick_port_delay(delay);
}

static inline void mp_tick_delayMs(unsigned int delay)
{
    return mp_tick_delay(MP_TICK_FROM_MS(delay));
}

#endif // MP_TICK_H
