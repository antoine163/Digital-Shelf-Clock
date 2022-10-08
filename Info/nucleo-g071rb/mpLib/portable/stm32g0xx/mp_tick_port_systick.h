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

#ifndef MP_TICK_PORT_BAREMETAL_H
#define MP_TICK_PORT_BAREMETAL_H

// Include ---------------------------------------------------------------------
#include "mpHardMap.h"

// St Low level
#include <stm32g0xx_ll_utils.h>

// Define macro ----------------------------------------------------------------
#define MP_TICK_PORT_MAX            (mp_tick_t)(-1)
#define MP_TICK_PORT_FROM_MS(ms)    ( ms * MP_TICK_RATE_HZ / 1000 )
#define MP_TICK_PORT_TO_MS(tick)    ( tick * 1000 / MP_TICK_RATE_HZ )

// Typedef ---------------------------------------------------------------------
typedef uint32_t mp_tick_t;

// Global variables ------------------------------------------------------------
extern volatile mp_tick_t _mp_tick_port_counter;

// Prototype functions ---------------------------------------------------------
void mp_tick_port_delay(mp_tick_t delay);

// Static inline functions -----------------------------------------------------
static inline int mp_tick_port_init()
{
    SysTick_Config( MP_TICK_CLOCK_HZ / MP_TICK_RATE_HZ );
    return 0;
}

static inline int mp_tick_port_deinit()
{
    SysTick->CTRL = 0;
    return 0;
}

static inline mp_tick_t mp_tick_port_get()
{
    return _mp_tick_port_counter;
}

static inline mp_tick_t mp_tick_port_getFromIsr()
{
    return _mp_tick_port_counter;
}



#endif // MP_TICK_PORT_BAREMETAL_H
